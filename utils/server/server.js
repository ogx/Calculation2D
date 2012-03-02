var http = require('http'),
	url = require('url'),
	bogart = require('bogart'),
	path = require('path'),
	viewEngine = bogart.viewEngine('mustache', path.join(__dirname, 'views')),
	router = bogart.router(),
	app = bogart.app(),
	q = bogart.q;

function debug_info() {
  
  console.log("\nMalleor, easy... it's just debugging messages\n");
  // print process.argv
  process.argv.forEach(function (val, index, array) {
    console.log('argument ' + index + ': ' + val);
  });

  console.log('process path: ' + process.execPath);
  console.log('cwd: ' + process.cwd());
}

// comment out if you don't want debug
debug_info();

var config = {
	serverAddress: 'localhost',
	serverPort: 8081,
	exePath: (process.argv.length > 2) ? 
		process.argv[2] : '../../bin/x64/Debug/calc2d.exe',
};


// Calculation2D inter-process API
var c2d = {
	// child process management
	child: null,
	child_busy: false,
	spawnProcess: function() {
		var spawn = require('child_process').spawn;
		this.child = spawn(config.exePath);
	},
	sendCommand: function(cmd, params) {
		//if(!this.child) not initialized...
		//if(this.child_busy) another task...
		
		this.child_busy = true;
		var deferred = q.defer(),
			accu_str = '',
			stdOut = this.child.stdout,
			onStdoutData = function(str) {
				//console.log('buffer length:', str.length);
				str = str.toString('ascii');
				var n = str.lastIndexOf('\n');
				if(n == str.length-1) {
					accu_str += str;
					//console.log('accu_str is now ', accu_str.length, 'and flushed.');
					deferred.resolve(JSON.parse(accu_str));
					this.child_busy = false;
					accu_str = '';
					stdOut.removeListener('data', onStdoutData);
				} else {
					//console.log('accu_str is now ', accu_str.length);
					accu_str += str;
				}
			};
		stdOut.on('data', onStdoutData);
		if(params && params.method) {
			params.method = params.method.replace(/ /g, '_');
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
};
c2d.spawnProcess();


// request routing (bogart)
router.get('/', function() {
	return viewEngine.respond('index.htm');
});

var promiseJson = function(obj_promise) {
		var json_promise = q.defer();
		q.when(obj_promise).then(function(obj_value) { 
			json_promise.resolve(bogart.json(obj_value));
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
	
var errorHandler = function(req) {
	return bogart.json({success:false, error:'server error'});
};
router.get(/\/.*/, errorHandler);
router.post(/\/.*/, errorHandler);

// http server launch
app.use(bogart.batteries);
app.use(router);
app.start(config.serverPort, config.serverAddress);