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
	exePath: '..\\x64\\Debug\\calc2d.exe',
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
		this.child.stdin.write(JSON.stringify({
			cmd: cmd,
			params: params ? params : {}
		})+'\r\n');
		return deferred.promise;
	},
	
	// c2d API
	getCalculationMethods: function() {
		return this.sendCommand('getmethods');
	},
	runCalculationMethod: function(method) {
		return this.sendCommand('run');
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
router.post('/test', function(req) {
	var x = req.params.image,
		image_base64 = x.slice(x.indexOf(',')+1),
		buf = new Buffer(image_base64, 'base64'),
		fs = require('fs'),
		fp = path.join(__dirname, 'uploaded.png');
	var ws = fs.createWriteStream(fp, {flags: 'w'});
	ws.end(buf);
	return bogart.json({
		result: 0 ? 'success' : 'failure'
	});
});

router.get('/getmethods', function(req) {
	var response = q.defer();
	q.when(c2d.getCalculationMethods()).then(function(value) { 
		//console.log(value.readSync());
		var json = bogart.json(value);
		//console.log(json);
		response.resolve(json);
	});
	return response;
});
router.get('/getstatus', function(req) {
	return bogart.json(c2d.getStatus());
});
router.post('/run', function(req) {
	return bogart.json(c2d.runCalculationMethod(req.params));
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