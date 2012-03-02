// server object provides a server API
var c2d_server = {
	test: function(image_base64) {
		$.ajax({
			url: '/test',
			type: 'post',
			data: {image: image_base64},
			accepts: 'json',
			success: function(response, statusText, xhr) {
				console.log(response.result);
			}
		});
	},
	
	getCalculationMethods: function(callback) {
		$.ajax({
			url: '/getmethods',
			type: 'get',
			data: {},
			accepts: 'json',
			success: function(response, statusText, xhr) {
				callback(response);
			}
		});
	},
	runCalculationMethod: function(method_id, method_params, image_structure, callback) {
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
	},
	getStatus: function(callback) {
		$.ajax({
			url: '/getstatus',
			type: 'get',
			data: {},
			accepts: 'json',
			success: function(response, statusText, xhr) {
				callback(response);
			}
		});
	}
};

// client object contains client-side stored data 
// and client data management functions
var c2d_client = {
	methods: [],
	setMethods: function(new_methods) {
		methods = new_methods.map(function(elem) {
			return elem;
		});
	},
	
	image_structure: [],
	newImage: function(image_data, image_name) {
		var image;
		if(image_data instanceof ImageData)
			image = {
				name: image_name,
				image: this.extractImage(image_data),
				data_layers: []
			};
		else
			image = {
				name: image_data.name,
				image: image_data.image,
				data_layers: image_data.data_layers || []
			};
		this.image_structure.push(image);
		
		surface.populateLists();
		surface.selectImage(this.image_structure.length-1);
	},
	extractImage: function(image_data) { // ImageData -> {w,h,r,g,b}
		var input = image_data.data,
			r = new Array(input.length/4),
			g = new Array(input.length/4),
			b = new Array(input.length/4);
		for(var i=0, j=0, n=input.length/4; i<n; i++, j++) {
			r[i] = input[j++]/255;
			g[i] = input[j++]/255;
			b[i] = input[j++]/255;
		}
		return {
			width: image_data.width,
			height: image_data.height,
			red: {
				name: 'red', 
				width: image_data.width, 
				height: image_data.height, 
				values: r
			},
			green: {
				name: 'green', 
				width: image_data.width, 
				height: image_data.height, 
				values: g
			},
			blue: {
				name: 'blue', 
				width: image_data.width, 
				height: image_data.height, 
				values: b
			},
		};
	},
	flattenImage: function(image, ctx) { // {w,h,r,g,b} -> ImageData
		var image_data = ctx.createImageData(image.width, image.height),
			output = image_data.data,
			r = image.values || image.red.values,
			g = image.values || image.green.values,
			b = image.values || image.blue.values;
		for(var i=0, j=0, n=output.length/4; i<n; i++) {
			output[j++] = r[i]*255;
			output[j++] = g[i]*255;
			output[j++] = b[i]*255;
			output[j++] = 255;
		}
		return image_data;
	},
	getImage: function(index, ctx) {
		return flattenImage(this.image_structure[index].image, ctx);
	},
	
	methods_lookup: {},
};

var surface = {
	// createForm
	// Create an HTML form for plugin parameters given in JSON. 
	createForm: function(default_params, parent) {
		var table = $(document.createElement('table')),
			createParamInput = this.createParamInput,
			that = this;
			
		default_params.forEach(function(param, i) {
			var input = that.createParamInput(param, 'param_'+i),
				label = $(document.createElement('label')).attr({
					'for': input.id,
					'class': 'param_label' 
				}).text(param.name),
				cell1 = $(document.createElement('td')),
				cell2 = $(document.createElement('td')),
				row = $(document.createElement('tr'));
				
			table.append(row.append(cell1.append(label)).append(cell2.append(input)));
			
			return row;
		});
		
		// display list of images
		this.populateLists($('#input_parameters_form select.image-list'));
		
		// TODO: layer-list for selected image
		
		return table;
	},
	
	// parseForm
	// Given the default parameters in JSON, parses the existing HTML form
	// and extracts parameter values. 
	// TODO: should validate values
	parseForm: function(default_params) {
		var factories = this.factories;
		return default_params.map(function(default_param, i) {
				var factory = factories[default_param.type] || factories['default'],
					parsed_param = $.extend(true, {}, default_param);
				parsed_param.value = factory.parse($('#param_'+i)[0]);
				return parsed_param;
			});
	},
	
	// populateLists
	// ...
	populateLists: function(list) {
		// image lists
		var image_options = c2d_client.image_structure.map(function(im, ind) {
			return $(document.createElement('option')).attr({
				'image-index': ind,
				value: ind
			}).text(im.name)[0];
		});
		if(!image_options.length)
			image_options.push($(document.createElement('option')).text('(no images)').attr('image-index', -1)[0]);
		var image_lists = $(list || 'select.image-list');
		image_lists.empty();
		image_lists.append(image_options);
		image_lists.change();
		
		// TODO: layer lists (for current image?)
	},
	
	selectImage: function(index) {
		$('#main_image_list').val(index.toString()).change();
	},
	
	// drawImageOnCanvas
	// ...
	drawImageOnCanvas: function(image) {
		var ctx = $('#input_preview_canvas')[0].getContext('2d');
		image_data = c2d_client.flattenImage(image, ctx);
		ctx.canvas.width = image.width;
		ctx.canvas.height = image.height;
		ctx.putImageData(image_data, 0, 0);
	},
	
	factories: new function() {
		var createEdit = function(param) {
				return $(document.createElement('input')).attr({
					type: 'text',
					value: param.value 
				})[0];
			},
			createCheckbox = function(param) {
				return $(document.createElement('input')).attr({
					type: 'checkbox',
					checked: param.value 
				})[0];
			},
			createCombo = function(cl) {
				return function(param) {
					return $(document.createElement('select')).attr({
						value: param.value,
						'class': cl
					})[0];
				};
			}, 
			getValue = function(node) {
				return $(node).val();
			},
			isChecked = function(node) {
				return node.checked;
			},
			getValueFromCombo = function(node) {
				return $('#'+node.id+' option:selected').text();
			},
			edit = {
				create: createEdit,
				parse: getValue
			}, 
			checkbox = {
				create: createCheckbox,
				parse: isChecked
			}, 
			combo = function(cl) {
				return {
					create: createCombo(cl),
					parse: getValueFromCombo
				};
			};
		return {
			'real': edit,
			'int': edit,
			'string': edit,
			'bool': checkbox,
			'image': combo('image-list'),
			'layer': combo('layer-list'),
			'default': edit
		};
	},
	createParamInput: function(param, id) {
		var factory = this.factories[param.type] || this.factories['default'];
		return $(factory.create(param, id)).attr('id', id)[0];
	},
};

$(document).ready(function() {
	// init appearances
	$('button').button();
	$('legend,label').addClass('header');
	$('#input_path').css({opacity: 0, height: 0});
	
	
	// input image handling
	$('#input_path').change(function() {
		// extract file 
		var file = $('#input_path')[0].files[0];
		
		// check for being an image
		if(!/image\/.*/.test(file.type))
			alert('Wrong file type (\''+file.type+'\').\nNeeds to be an image.');
		
		// load the file to <img>
		var fr = new FileReader();
		fr.onloadend = function(e) {
			var ip = $('#input_preview'),
				data = e.target.result;
			
			var img = new Image();
			img.onload = function() {
				var canvas = $('#input_preview_canvas')[0],
					ctx = canvas.getContext('2d');
				ctx.canvas.width = img.width;
				ctx.canvas.height = img.height;
				ctx.drawImage(img, 0, 0);
				
				c2d_client.newImage(
					ctx.getImageData(0, 0, img.width, img.height),
					file.name
				);
			}
			img.src = data;
		};
		fr.readAsDataURL(file);
	});
	
	
	// plug in server's API
	$('#get_methods').click(function() {
		c2d_server.getCalculationMethods(function(methods){
			c2d_client.setMethods(methods);
			
			// populate combo
			var combo = $('#available_methods');
			combo.empty();
			methods.forEach(function(method) {
				var option = $(document.createElement('option')),
					method_text = method.author + ': ' + method.name;
				option.text(method_text);
				option.attr('method_id', method.id);
				c2d_client.methods_lookup[method.id] = method;
				combo.append(option);
			});
			
			// handle detailed view
			combo.change(function() {
				var option = $('#available_methods option:selected'),
					method_id = option.attr('method_id'),
					method = c2d_client.methods_lookup[method_id];
				if(!method) {
					$('#method_details').hide();
					throw new Exception('Selected method not found!');
				}
				
				var max_params_length = 150;
				$('#method_details #name').text(method.name);
				$('#method_details #author').text(method.author);
				$('#method_details #description').text(method.description);
				$('#method_details #input_parameters').html(
					'<div id="input_parameters_code"/><div id="input_parameters_form"/>'
					);
				$('#input_parameters_code').text(
					JSON.stringify(method.params.in, null, " ")
					);
				$('#input_parameters_form').empty().append(
					surface.createForm(method.params.in)
					);
				$('#method_details #output_parameters').html(
					'<div id="input_parameters_code"/><div id="input_parameters_form"/>'
					);
				$('#output_parameters_code').text(
					JSON.stringify(method.params.out, null, " ")
					);
				$('#output_parameters_form').empty().append(
					surface.createForm(method.params.out)
					);
				$('#method_details').show();
			});
			combo.trigger('change');
		});
	});
	$('#run_method').click(function() {
		var option = $('#available_methods option:selected'), 
			method_id = option.attr('method_id'),
			method_params = c2d_client.methods_lookup[method_id].params.in,
			image_structure = $('#input_parameters_form select.image-list').map(function() { // get all images selected by user as input parameters
				return c2d_client.image_structure[$(this).children(':selected').attr('image-index')];
			}).get(),
			runMethod = function(method_id, method_params, image_structure) {
				/*console.log('Launching method', method_id);
				console.log('  -method_params:', method_params);
				console.log('  -structure:', image_structure);*/
				c2d_server.runCalculationMethod(
					method_id, 
					method_params,
					image_structure,
					function(res){
						console.log('runCalculationMethod returned', res);
						if(res.success)
						{
							// calculation launched.
							var interval = setInterval(function() {
								c2d_server.getStatus(function(res){
									if(res.status === 'finished')
										clearInterval(interval);
										
									// results arrived.
									console.log('getStatus returned', $.extend(true, {}, res));
									
									res.result.image_structure.forEach(function(image) {
										c2d_client.newImage(image);
									});
								});
							}, 100);
						}
					}
				);
			};
			
		// parse params
		method_params = surface.parseForm(method_params) || [];
		
		// launch calculation
		runMethod(method_id, method_params, image_structure);
	});
	$('#get_status').click(function() {
		c2d_server.getStatus(function(res){
			console.log(res);
		});
	});
	$('#test').click(function() {
		var image = c2d_client.image_structure[c2d_client.selected_image].image,
			ch = image.blue;
		for(var i=0; i<ch.length; i++)
			ch[i] /= 2;
		var ctx = $('#input_preview_canvas')[0].getContext('2d');
		image_data = c2d_client.flattenImage(image, ctx);
		ctx.putImageData(image_data, 0, 0);
	});
	$('#test2').click(function() {
		var canvas = $('canvas')[0],
			url = canvas.toDataURL();
		console.log('Currently displayed image\'s URL-encoded content:', url);
	});
	$('#load_btn').click(function() {
		$('#input_path').trigger('click');
	});
	
	$('#main_image_list').change(function() {
		var option = $(this).children('option:selected'),
			image_index = parseInt(option.attr('image-index')),
			image = c2d_client.image_structure[image_index];
		//console.log(option, '-> index', image_index, '-> image', image);
		if(image)
			surface.drawImageOnCanvas(image.image);
	});
	
	surface.populateLists();
});