var http = require('http'),
	url = require('url'),
	bogart = require('bogart'),
	path = require('path'),
	viewEngine = bogart.viewEngine('mustache', path.join(__dirname, 'views')),
	router = bogart.router(),
	app = bogart.app(),
	q = bogart.q;


var config = {
	serverAddress: 'localhost',
	serverPort: 8081,
	exePath: process.argv[2] || '../../bin/x64/Debug/calc2d.exe',
};

process.nextTick(function() {
	console.log("\n Calculation2D server functional. \n Go to the browser and type: " +
		"\n   http://" + config.serverAddress + ":" + config.serverPort + "\n");
		
	var print_debug_info = function() {
		console.log('process path: ' + process.execPath);
		console.log('cwd: ' + process.cwd());
		process.argv.forEach(function (val, index, array) {
			console.log('argument ' + index + ': ' + val);
		});
	};
	//print_debug_info();
});


// Calculation2D inter-process API
var c2d = {
	// child process management
	child: null,
	child_busy: false,
	child_finalizing: false,
	receiveFromChild: function(stream, callback) {
		var accumulated = '', 
			listener = function(partial) {
				//console.log('buffer length:', partial.length);
				partial = partial.toString('ascii');
				var n = partial.lastIndexOf('\n');
				if(n == partial.length-1) {
					accumulated += partial;
					//console.log('accumulated is now ', accumulated.length, 'and flushed.');
					
					callback(accumulated);
					
					this.child_busy = false;
					accumulated = '';
					stream.removeListener('data', listener);
				} else {
					//console.log('accumulated is now ', accumulated.length);
					accumulated += partial;
				}
			};
		this.child_busy = true;
		stream.on('data', listener);
	},
	spawnProcess: function() {
		var spawn = require('child_process').spawn,
			errorListener, 
			registerErrorListener, 
			self = this,
			spawnChild = function() {
				// spawn
				self.child = spawn(config.exePath);
				
				// error listening
				registerErrorListener = function() {
					self.receiveFromChild(self.child.stderr, errorListener);
				};
				errorListener = function(error_line) {
					console.error('Calculation server indicated an error: \n'+error_line);
					registerErrorListener();
				};
				registerErrorListener();
				
				// crash control
				self.child.on('exit', function(code, signal) {
					if(code)
						console.error('Calculation server exited with code '+code+'.');
					if(signal)
						console.error('Calculation server has been killed with the signal: \''+code+'\'.');
					self.child = null;
					if(!self.child_finalizing) {
						console.log('Respawning.');
						spawnChild();
					}
				});
			};
		spawnChild();
	},
	sendCommand: function(cmd, params) {
		if(!this.child) {
			console.warn('The command \''+cmd+'\' will not be sent. The child process is not running.');
			var deferred = q.defer();
			deferred.resolve({
				success: false,
				error: 'Calculation server crashed.'
			});
			return deferred.promise;
		}
		//if(this.child_busy) another task...
		
		var deferred = q.defer(),
			stdOut = this.child.stdout,
			onStdoutData = function(accu_str) {
				try {
					deferred.resolve(JSON.parse(accu_str));
				} catch(e) {
					var accu_str_ws = accu_str.replace(/\s/g, '.');
					console.error('Caught error \'', e, '\'.\n'+
						'The object being parsed is:\n', accu_str_ws);
				}
			};
		this.receiveFromChild(stdOut, onStdoutData);
		if(params && params.method) {
			//params.method = params.method.replace(/ /g, '_');
			//console.log('sent exactly:', JSON.stringify(params.method));
		}
		var command = JSON.stringify({
				cmd: cmd,
				params: params || {}
			})+'\r\n';
		this.child.stdin.write(command);
		return deferred.promise;
	},
	
	// c2d API
	getCalculationMethods: function() {
		return this.sendCommand('getmethods');
	},
	runCalculationMethod: function(params) {
		return this.sendCommand('run', params);
	},
	getStatus: function() {
		return this.sendCommand('getstatus');
	},
	finalize: function() {
		this.child_finalizing = true;
		return this.sendCommand('finalize');
	},
};
c2d.spawnProcess();


// request routing (bogart)
router.get('/old', function() {
	return viewEngine.respond('index.htm', { 
		locals: {
			styles: ['index'], 
			jquery: true, 
			jqueryui: true
		} 
	});
});
router.get('/', function() {
	return viewEngine.respond('index-new.htm', { 
		locals: {
			jquery: true, 
			jqueryui: false,
			styles: ['index-new'], 
			headers: [
				"<link href='http://fonts.googleapis.com/css?family=Open+Sans+Condensed:700,300' rel='stylesheet' type='text/css'>",
				"<link href='http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700,800' rel='stylesheet' type='text/css'>"
			],
		} 
	});
});

var promiseJson = function(obj_promise, opt_callback) {
		var json_promise = q.defer();
		q.when(obj_promise).then(function(obj_value) { 
			json_promise.resolve(bogart.json(obj_value));
			if(opt_callback)
				opt_callback();
		});
		return json_promise;
	};
router.get('/getmethods', function(req) {
	return promiseJson(c2d.getCalculationMethods());
});
router.get('/getstatus', function(req) {
	return promiseJson(c2d.getStatus());
});
router.post('/run', function(req) {
	return promiseJson(c2d.runCalculationMethod(req.params));
});
router.post('/finalize', function(req) {
	console.log('Being finalized by the client-side...');
	console.log('  Finalizing the child...');
	return promiseJson(c2d.finalize(), function() {
		console.log('  ...child finalized. Exiting myself.');
		process.exit(0);
	});
});
	
var errorHandler = function(req) {
	return bogart.json({success:false, error:'server error'});
};
router.get(/\/.*/, errorHandler);
router.post(/\/.*/, errorHandler);

// http server launch
app.use(bogart.batteries);
app.use(router);
app.start(config.serverPort, config.serverAddress);