/*
 *	This module defines a client GUI logic. It encapsulates 
 *	utilization of jQuery and overall interaction with the user. 
 *
 *	Architectural note:
 *		Event entry points are first-level functions of 
 *		C2D.surface. They are dispatched to one or more 
 *		sub-modules (sub-objects).
 *
 *	Interfaces: 
 *		- ...
 */
 
var C2D = C2D || {};


C2D.surface = {
	init: function() {
		var self = this;
		
		C2D.server.init();
		C2D.client.init();
		
		
		/* PRIVATE SECTION */
		var method_id_key = 'method-id',
			resource_id_key = 'resource-id';
		
		
		// sub-module for creation and parsing parameter forms
		this.param_form_mgr = {
			surface: C2D.surface,
			
			factories: new (function() {
				var createEdit = function(param) {
						return $(document.createElement('input')).attr({
							type: 'text',
							value: param.value,
							'class': 'wide-ctl',
						})[0];
					},
					createCheckbox = function(param) {
						return $(document.createElement('input')).attr({
							type: 'checkbox',
							checked: param.value,
						})[0];
					},
					createCombo = function(cl) {
						return function(param) {
							return $(document.createElement('select')).attr({
								value: param.value,
								'class': cl+' wide-ctl',
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
			})(),
			
			createParamInput: function(param, id) {
				var factory = this.factories[param.type] || this.factories['default'];
				return $(factory.create(param, id)).attr('id', id)[0];
			},
			
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
						cell1 = $(document.createElement('td')).addClass('param-label-cell'),
						cell2 = $(document.createElement('td')).addClass('param-value-cell'),
						row = $(document.createElement('tr'));
						
					table.append(row.append(cell1.append(label)).append(cell2.append(input)));
					
					return row;
				});
				
				// display list of images
				this.surface.image_structure_reflection.populateImageLists($('#input_parameters_form select.image-list'));
				
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
		};
		
		// sub-module for managing synchronization of client.image_structure and and UI controls
		this.image_structure_reflection = {
			populateImageLists: function(elements) {
				// get selected elements
				var ids = [];
				for(var i=0, n=elements.length; i<n; i++) {
					ids[i] = $(elements[i]).children(':selected').attr(resource_id_key);
				}
				//console.log('combos:', elements, 'selected items:', ids);
				
				// populate lists
				elements.empty();
				elements.append(this.image_options.length ? 
								this.image_options : 
								this.createEmptyImageListOption());	
				
				// restore selections
				for(var i=0, n=elements.length; i<n; i++) {
					var option_to_select = $(elements[i]).
							children('option[' + resource_id_key + '=' + ids[i] + ']');
					if(option_to_select.length == 0) { // already checked if no options
						option_to_select.children().first();
					}
					option_to_select.attr('selected', 'selected');
				}
					
				elements.change();
			},
			populateLayerLists: function(elements, image) {
				var options = [];
				if(typeof image === 'undefined') {
					// TODO: get options for a specific image (use layer_options)
				} else {
					// TODO: populate global layers
				}
				elements.empty();
				elements.append(options);
				elements.change();
			},
			
			createImageListOption: function(id, name) {
				return $(document.createElement('option')).
					attr(resource_id_key, id).
					attr('selected', 'selected').
					text(name)[0];
			},
			createEmptyImageListOption: function() {
				return this.createImageListOption(-1, '(no images)');
			},
			
			image_options: [],
			layer_options: {},
			onImageStructureChanged: function() {
				var self = this;
				
				// image lists
				this.image_options = [];
				C2D.client.forImages(function(im) {
					self.image_options.push(self.createImageListOption(im.id, im.name));
				});
				this.populateImageLists($('select.image-list'));
				
				// TODO: layer lists (for current image?)
			},
			onLayerListsChanged: function() {
				populateLayerLists($('select.layer-list'));
			},
			
			selectImage: function(image_id) {
				var image_list = $('#main_image_list'),
					option = image_list.
							 children('option[' + resource_id_key + '=' + image_id + ']').
							 attr('selected', 'selected').
							 change();
			},
			
			drawImageOnCanvas: function(image) {
				var ctx = $('#input_preview_canvas')[0].getContext('2d');
				image_data = image.getRGBA();
				ctx.canvas.width = image_data.width;
				ctx.canvas.height = image_data.height;
				ctx.putImageData(image_data, 0, 0);
			},
		};
		
		// input image handling
		this.onFileSelected = function() {
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
					var image = C2D.client.loadImage(img, file.name);
					self.image_structure_reflection.selectImage(image.id);
				}
				img.src = data;
			};
			fr.readAsDataURL(file);
		};
		
		// method details handling
		this.onMethodSelected = function() {
			var option = $('#available_methods option:selected'),
				method_id = option.attr(resource_id_key),
				method = C2D.client.getMethod(method_id);
			if(!method) {
				$('#method_details').hide();
				throw new Exception('Selected method not found!');
			}
			
			var max_params_length = 150;
			$('#method_details #name').text(method.name);
			var author = (method.author.first_name || method.author.last_name) ? 
						  method.author.first_name + ' ' + method.author.last_name :
						  'Anonymous',
				email = method.author.email ? ' ('+method.author.email+')' : '';
			$('#method_details #author').text(author+email);
			$('#method_details #description').text(method.description);
			$('#input_parameters').html(
				'<div id="input_parameters_code debug"/><div id="input_parameters_form"/>'
				);
			$('#input_parameters_code').text(
				JSON.stringify(method.params.in, null, " ")
				);
			$('#input_parameters_form').empty().append(
				C2D.surface.param_form_mgr.createForm(method.params.in)
				);
			$('#output_parameters').html(
				'<div id="input_parameters_code"/><div id="input_parameters_form"/>'
				);
			$('#output_parameters_code').text(
				JSON.stringify(method.params.out, null, " ")
				);
			$('#output_parameters_form').empty().append(
				C2D.surface.param_form_mgr.createForm(method.params.out)
				);
			$('#method_details').show();
		};
		
		// method-lists update needed
		this.onMethodsReceived = function(methods) {
			// populate combo
			var combo = $('#available_methods');
			combo.empty();
			methods.forEach(function(method) {
				var option = $(document.createElement('option')),
					author = (method.author.first_name || method.author.last_name) ? 
							 method.author.first_name + ' ' + method.author.last_name :
							 'Anonymous',
					method_text = author + ': ' + method.name;
				option.text(method_text);
				option.attr(resource_id_key, method.id);
				C2D.client.addMethod(method);
				combo.append(option);
			});
			
			// handle detailed view
			combo.change();
		};
		
		// 
		this.onImageListChanged = function(list) {
			var option = $(list).children('option:selected'),
				image_id = option.attr(resource_id_key),
				image = C2D.client.findImage(image_id);
			if(image)
				self.image_structure_reflection.drawImageOnCanvas(image);
		};
		
		// user wants to launch an algorithm
		this.onRunMethod = function() {
			var option = $('#available_methods option:selected'), 
				method_id = option.attr(resource_id_key),
				method_params = C2D.client.getMethod(method_id).params.in,
				
				selected_image_ids = $('#input_parameters_form select.image-list option:selected').map(function() {
					return $(this).attr(resource_id_key);
				}).get(),
				wrapped_input_structure = C2D.client.wrapStructure(selected_image_ids),
				
				status_interval,
				getStatus = function() {
					C2D.server.getStatus(function(response) {
						console.log('getStatus returned', $.extend(true, {}, response));
						
						if(!response.success) {
							clearInterval(status_interval);
							self.onCalculationNotRunning();
						} else if(response.finished) {
							clearInterval(status_interval);
							self.onCalculationFinished(response.result);
						} else {
							self.onCalculationProgress(response.progress);
						}
					});
				},
				onMethodResponse = function(response) {
					console.log('runCalculationMethod returned', response);
					
					if(response.success) {
						status_interval = setInterval(getStatus, 100);
					}
				},
				runMethod = function(method_id, method_params, image_structure) {
					C2D.server.runCalculationMethod(
						method_id, 
						method_params,
						image_structure,
						onMethodResponse
					);
				};
				
			console.log('selected IDs:', selected_image_ids);
			console.log('wrapped structure:', wrapped_input_structure);
				
			// parse params
			method_params = self.param_form_mgr.parseForm(method_params) || method_params;
			
			// launch calculation
			runMethod(method_id, method_params, wrapped_input_structure);
		};
		
		// calculation status reporting functions
		this.onCalculationProgress = function(status, progress) {
			// TODO: impl
		}
		this.onCalculationFinished = function(results) {
			// TODO: impl
		}
		this.onCalculationNotRunning = function() {
			// TODO: impl
		}
		
		/* HOOK UP TO THE DOM */
		$('#input_path').change(function() {
			self.onFileSelected();
		});
		$('#load_btn').click(function() {
			$('#input_path').trigger('click');
		});
		$('#available_methods').change(function() {
			self.onMethodSelected();
		});
		$('#main_image_list').change(function() {
			self.onImageListChanged(this);
		});
		
		/* server's API */
		$('#get_methods').click(function() {
			C2D.server.getCalculationMethods(function(methods) {
				self.onMethodsReceived(methods);
			});
		}).click();
		$('#run_method').click(function() {
			self.onRunMethod();
		});
		$('#get_status').click(function() {
			C2D.server.getStatus(function(res){
				console.log(res);
				if(!res.success) {
					self.onCalculationNotRunning();
				} else if(res.finished) {
					self.onCalculationFinished(res.result);
				} else {
					self.onCalculationProgress(res.progress);
				}
			});
		});
		$('#finalize_btn').click(function() {
			C2D.server.finalize();
		});
		
		
		this.image_structure_reflection.onImageStructureChanged();
		
		delete this.init;
	}
};


// the only on-load hook
$(document).ready(function() {
	C2D.surface.init();
});