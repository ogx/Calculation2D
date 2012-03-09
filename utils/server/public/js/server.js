/*
 *	This module defines a server (http) interface for the client. 
 *
 *	Interfaces: 
 *		- get available algorithms
 *		- run an algorithm
 *		- get processing status
 *		- server shutdown
 */
 
var C2D = C2D || {};


C2D.server = {
	init: function() {		
		// TODO: server handshake/initialization

		// get available algorithms
		this.getCalculationMethods = function(callback) {
			$.ajax({
				url: '/getmethods',
				type: 'get',
				data: {},
				accepts: 'json',
				success: function(response, statusText, xhr) {
					callback(response);
				}
			});
		};
		
		// queues execution of given algorithm on given data with given parameters
		this.runCalculationMethod = function(method_id, method_params, image_structure, callback) {
			console.log('Launching method', method_id);
			console.log('  -method_params:', method_params);
			console.log('  -structure:', image_structure);
			
			$.ajax({
				url: '/run',
				type: 'post',
				data: {
					method: method_id,
					method_params: method_params,
					image_structure: image_structure
				},
				accepts: 'json',
				success: function(response, statusText, xhr) {
					callback(response);
				}
			});
		};
		
		// get current execution status
		this.getStatus = function(callback) {
			$.ajax({
				url: '/getstatus',
				type: 'get',
				data: {},
				accepts: 'json',
				success: function(response, statusText, xhr) {
					callback(response);
				}
			});
		};
		
		// shuts the server down
		this.finalize = function() {
			$.ajax({
				url: '/finalize',
				async: true,
				type: 'post',
				data: { reason: 'The user wishes so.' },
				accepts: 'json',
				success: function(response, statusText, xhr) {
					if(!response.success)
						alert('Server could not finalize.');
					else
						alert('Server finalized.');
				}
			});
		};
		
		// transport utilities
		this.compress = function(array) {
			return btoa(array.reduce(function(pv, cv) { return pv + String.fromCharCode(cv*255); }, ''));
		};
		this.decompress = function(str) {
			var decoded = atob(str),
				len = decoded.length,
				array = new Array(len);
			for(var i=0; i<len; i++)
				array[i] = decoded.charCodeAt(i)/255;
			return array;
		};
		
		delete this.init;
	}
};