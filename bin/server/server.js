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
	exePath: '..\\x64\\Debug\\calc2d.exe', // TODO: parametrize
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
		var deferred = q.defer();
		this.child.stdout.once('data', function(str) {
			// TODO: it's likely the stream buffer can overflow and we should 
			// accumulate the response till, say, crlf
			deferred.resolve(JSON.parse(str));
			this.child_busy = false;
		});
		if(params && params.method) {
			params.method = params.method.replace(/ /g, '_');
			//console.log('sent exactly:', JSON.stringify(params.method));
		}
		var command = JSON.stringify({
				cmd: cmd,
				params: params ? params : {}
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