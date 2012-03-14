/*
 *	This module defines a client-side image structure. 
 *
 *	Interfaces: 
 *		- import image from an Image object
 *		- import image/layer from a base64 buffer
 *		- export image to a Canvas buffer
 *		- export image/layer to a base64 buffer
 *		- wrap image structure to be sent to the server
 *		- unwrap server response data
 *		- access the numbers, naturally
 */

var C2D = C2D || {};


C2D.Layer = function(name, width, height, values, def_value, draw_buffer) {
	var next_layer_id = 1;
	C2D.Layer = function(name, width, height, values, def_value, draw_buffer) {
		this.id = '' + (next_layer_id++);
		this.name = name || '';
		this.width = width || 0;
		this.height = height || 0;
		this.values = values || [];
		this.def_value = def_value || 0;
		
		var draw_buffer = draw_buffer || console.error('The layer needs a draw_buffer!');
		
		this.getStats = function() {
			var min, max, mean = 0, 
				v = this.values, 
				vv, nv = 0,
				dv = this.def_value;
				
			for(var i=0, n=width*height; i<n; i++)
				if(v[i] != dv) {
					min = max = v[0];
					break;
				}
			if(typeof min === 'undefined')
				return {
					min: NaN,
					max: NaN,
					mean: NaN
				};
				
			for(var i=0, n=width*height; i<n; i++) {
				vv = v[i];
				if(vv != dv) 
					continue;
				if(vv > max) max = vv;
				else if(vv < min) min = vv;
				mean += vv;
				nv++;
			}
			mean /= nv;
			
			return {
				min: min,
				max: max,
				mean: mean
			};
		};
		this.getRGBA = function() {
			console.log('layer draw_buffer:', draw_buffer);
			var y = this.values,
				dv = this.def_value,
				stats = this.getStats(),
				rgba = draw_buffer.data;
			console.log('layer stats:', stats);
			for(var i=0, j=0, v, n=width*height; i<n; i++){
				v = y[i];
				if(v == dv) { // default value
					rgba[j++] = 0x7F;
					rgba[j++] = 0x47;
					rgba[j++] = 0x7F;
				} else { // grayscale layer display; TODO: switchable color scales
					v *= 0xFF;
					rgba[j++] = v;
					rgba[j++] = v;
					rgba[j++] = v;
				}
				rgba[j++] = 0xFF;
			}
			return draw_buffer;
		};
	};
	return new C2D.Layer(name, width, height, values, def_value, draw_buffer);
};

C2D.Image = function(name, width, height, content) {
	var next_image_id = 1;
	C2D.Image = function(name, width, height, content) {
		/* PRIVATE SECTION */
		var draw_buffer = {}, 
			channels = [],
			data_layers = {},
			
			wrapLayer = function(channel) {
				return {
					id: channel.id,
					name: channel.name,
					width: channel.width,
					height: channel.height,
					values: C2D.server.compress(channel.values),
					default_value: channel.def_value
				};
			},
			unwrapLayer = function(channel) {
				return new C2D.Layer(
					channel.name,
					channel.width,
					channel.height,
					C2D.server.decompress(channel.values),
					channel.default_value,
					draw_buffer
				);
			};
				
		
		/* CONSTRUCTION */
		if(content.channels) {
			// (convert from a server-compatible object)
			// - import channels
			// - create a buffer
			
			draw_buffer = C2D.surface.image_structure_reflection.
				getCanvasContext().createImageData(width, height); // TODO: needs to be done differently - this is a mess
			
			channels = content.channels.map(unwrapLayer);
		} else if(content.imade_data) {
			// (create from ImageData canvas buffer)
			// - extract RGB channels from canvas buffer
			// - save the buffer for this.getRGBA
			
			var canvas_data = content.imade_data,
				rgba = canvas_data.data,
				r = new Array(rgba.length/4),
				g = new Array(rgba.length/4),
				b = new Array(rgba.length/4);
			
			for(var i=0, j=0, n=rgba.length/4; i<n; i++, j++) {
				r[i] = rgba[j++]/255;
				g[i] = rgba[j++]/255;
				b[i] = rgba[j++]/255;
			}
			
			channels = [r, g, b];
			draw_buffer = canvas_data;
		
			var num_channels = channels.length,
				channel_names = [];
				
			if(num_channels == 1)
				channel_names = ['I'];
			else if(num_channels == 3)
				channel_names = ['R', 'G', 'B'];
			else if(num_channels == 4)
				channel_names = ['R', 'G', 'B', 'A'];
			else
				throw new Error('Crazy channel count ('+num_channels+')!');
				
			channels = channels.map(function(arr) {
				return new C2D.Layer(
					channel_names[this.i++],
					width, 
					height,
					arr,
					-1,
					draw_buffer
				);
			}, {i:0});
		} else 
			throw new Error('No apropriate content for the image.');
		
		
		var self = this;
		

		this.id = '' + (next_image_id++);
		this.name = name || '(no-name)';
		this.width = width; 
		this.height = height;
		
		// converts the image to be displayed on a canvas
		this.getRGBA = function() {
			var rgba = draw_buffer.data;
			
			if(channels.length == 1) {
				var y = channels[0].values;
				for(var i=0, v, n=width*height; i<n; i++){
					v = y[i]*0xFF;
					rgba[j++] = v;
					rgba[j++] = v;
					rgba[j++] = v;
					rgba[j++] = 0xFF;
				}
			} else if(channels.length == 3) {
				var r = channels[0].values,
					g = channels[1].values,
					b = channels[2].values;
				for(var i=0, j=0, n=width*height; i<n; i++) {
					rgba[j++] = r[i]*0xFF;
					rgba[j++] = g[i]*0xFF;
					rgba[j++] = b[i]*0xFF;
					rgba[j++] = 0xFF;
				}
			} else if(channels.length == 4) {
				var r = channels[0].values,
					g = channels[1].values,
					b = channels[2].values,
					a = channels[3].values;
				for(var i=0, j=0, n=width*height; i<n; i++) {
					rgba[j++] = r[i]*0xFF;
					rgba[j++] = g[i]*0xFF;
					rgba[j++] = b[i]*0xFF;
					rgba[j++] = a[i]*0xFF;
				}
			} else
				throw new Error('Wrong channel count! Shouldn\'t happen!');
				
			return draw_buffer;
		};
		
		/* LAYER MANIPULATION */
		this.getLayer = function(id) {
			return data_layers[id];
		};
		this.forEachChannel = function(callback) {
			channels.forEach(callback);
		};
		this.forEachLayer = function(callback) {
			for(var id in data_layers) {
				callback(data_layers[id]);
			}
		};
		this.mapLayers = function(callback) {
			var mapped = [];
			for(var id in data_layers) {
				mapped.push(callback(data_layers[id]));
			}
			console.log(data_layers, '->', mapped);
			return mapped;
		};
		
		// wraps itself to a server-compatible form
		this.wrapItUp = function() {
			return {
				id: self.id,
				name: self.name,
				width: self.width,
				height: self.height,
				channels: channels.map(wrapLayer),
				data_layers: self.mapLayers(wrapLayer),
			};
		};
	};
	return new C2D.Image(name, width, height, content);
};

C2D.ImageStructure = function() {
	var images = this.images = {};
	
	// loads the image from an Image object
	this.loadImage = function(img, name) {
	    var width = img.width,
			height = img.height,
			mem_canvas = document.createElement('canvas');
			mem_canvas.width = width;
			mem_canvas.height = height;
			ctx = mem_canvas.getContext('2d'),
			_x = ctx.drawImage(img, 0, 0),
			canvas_data = ctx.getImageData(0, 0, width, height);
		
		var image = new C2D.Image(
				name, 
				width, 
				height,
				{imade_data: canvas_data}
			);
		images[image.id] = image;
		return image;
	};
	
	// wraps the structure for transport to the server
	this.wrapItUp = function(selected_image_ids) {
		return selected_image_ids.map(function(id) {
			return images[id].wrapItUp();
		});
	};
	
	// merge processing results from the server
	this.digestResults = function(results) {
		var diff = {
				new_images: []
			};
		results.forEach(function(result_image) {
			var new_image = new C2D.Image(
					result_image.name, 
					result_image.width, 
					result_image.height, 
					{ channels:    result_image.channels, 
					  data_layers: result_image.data_layers }
				);
			console.log('Retrieved an image:', new_image);
			images[new_image.id] = new_image;
			diff.new_images.push(new_image);
		});
		return diff;
	};
	
	/* image manipulation */
	this.findImage = function(id) {
		return images[id];
	};
	this.forEachImage = function(callback) {
		for(var id in images) {
			callback(images[id]);
		}
	};
};

if(typeof module !== 'undefined')
	module.exports = C2D;