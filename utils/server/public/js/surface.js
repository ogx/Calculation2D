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
		
		
		/* PRIVATE ELEMENTS */
		
		var method_id_key = 'method-id',
			resource_id_key = 'resource-id',
			
			image_options = [],
			layer_options = {},
			populateLists = function(elements, options) {
				// get selected elements
				var ids = [];
				for(var i=0, n=elements.length; i<n; i++) {
					ids[i] = $(elements[i]).children(':selected').attr(resource_id_key);
				}
				//console.log('combos:', elements, 'selected items:', ids);
				
				// populate lists
				elements.empty();
				elements.append(options.length ? 
								options : 
								createEmptyListOption());	
				
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
			createListOption = function(id, name) {
				return $(document.createElement('option')).
					attr(resource_id_key, id).
					text(name)[0];
			},
			createEmptyListOption = function() {
				return createListOption(-1, '(no data)');
			},
			
			// html elements reflection -- image/layer lists, parameters, canvas
			html_types = {
				ImageItem: function(image) {
					image = image || { list_item_id: -1, name: '(no images)' };
					this.element = $(document.createElement('option')).
						attr(resource_id_key, image.list_item_id).
						text(image.name);
					this.object = image;
					this.draw = function(ctx) {
						ctx.canvas.width = this.object.width;
						ctx.canvas.height = this.object.height;
						ctx.putImageData(this.object.getRGBA(), 0, 0);
					};
				},
				LayerItem: function(layer) {
					layer = layer || { list_item_id: -1, name: '(no layers)' };
					this.element = $(document.createElement('option')).
						attr(resource_id_key, layer.list_item_id).
						text(layer.name);
					this.object = layer;
					this.draw = function(ctx) {
						ctx.canvas.width = this.object.width;
						ctx.canvas.height = this.object.height;
						ctx.putImageData(this.object.getRGBA(), 0, 0);
					};
				},
				
				GenericList: function(element, item_ctor) {
					element = $(element).get(0) || document.createElement('select');
					if(element.list_object)
						return element.list_object;
					element.list_object= this;
					//console.log('GenericList from', element);
					var empty_item = new item_ctor(),
						items = this._i = {}
					
					// fields
					this.element = $(element);
					// methods
					this.add = function(item_object) {
						if($.isEmptyObject(items))
							this.element.empty();
					
						if(typeof html_types.GenericList.next_id === 'undefined')
							html_types.GenericList.next_id = 0;
						item_object.list_item_id = item_object.list_item_id || html_types.GenericList.next_id++;
						
						if(typeof items[item_object.list_item_id] !== 'undefined') {
							console.warn('List item object already registered. Replacing.');
							delete items[item_object.list_item_id];
						}
							
						var item = new item_ctor(item_object);
						items[item_object.list_item_id] = item;
						this.element.append(item.element);
						return item;
					};
					this.remove = function(item_object) {
						var item = items[item_object.list_item_id];
						this.element.remove(item.element);
						delete items[item_object.list_item_id];
						
						if($.isEmptyObject(items))
							this.element.empty().append(empty_item.element);
					};
					this.empty = function() {
						for(var id in items)
							delete items[id];
					};
					this.getSelectedItem = function() {
						var selected = $(this.element).
								children('option:selected'),
							list_item_id = selected.attr(resource_id_key);
						if(list_item_id == -1)
							return {
								element: selected,
								object: null,
								draw: function(ctx) {
									var w = ctx.canvas.width, 
										h = ctx.canvas.height;
									var pfs = ctx.fillStyle;
									ctx.fillStyle = $(ctx.canvas).css('background-color');
									ctx.fillRect(0, 0, w, h);
									ctx.fillStyle = pfs;
								}
							};
						return items[list_item_id];
					};
					this.getSelected = function() {
						return this.getSelectedItem().object;
					};
					this.select = function(id) {
						this.element.
							children('option['+resource_id_key+'='+id+']').
							attr('selected', 'selected');
						this.element.change();
					};
					
					this.element.empty().append(empty_item.element);
				},
				ImageList: function(element) {
					var list = new html_types.GenericList(element, html_types.ImageItem);
					
					// populate with all images
					if(C2D.client.getImagesCount() > 0) {
						list.element.empty();
						C2D.client.forImages(function(image) {
							list.add(image);
						});
					}
					
					return list;
				},
				LayerList: function(element) {
					var list = new html_types.GenericList(element, html_types.LayerItem);
					
					// additional methods
					list.populate = function(image) {
						console.log(image);
						list.element.empty();
						list.empty();
						list.add(image).element.text('(image)'); // RGBA image
						image.forEachChannel(function(channel) {
							var item = list.add(channel);
							item.element.text('('+channel.name+' channel)');
						});
						image.forEachLayer(function(layer) {
							list.add(layer);
						});
						
						list.element.children().first().attr('selected', true);
						list.element.change();
						
						return list;
					};
					
					return list;
				},
			},
			_main_image_list = new html_types.ImageList($('#main_image_list')),
			_main_layer_list = new html_types.LayerList($('#main_layer_list')),
			_main_canvas = $('#input_preview_canvas'),
			html_objects = C2D._h = {				
				// persistent controls
				main_canvas: {
					element: _main_canvas,
					context: _main_canvas[0].getContext('2d'),
					
					drawLayer: function(layer) {
						throw new Error('Not implemented yet.');
					},
					drawImage: function(image) {
						var image_data = image.getRGBA();
							canvas = $(this.element)[0];
						canvas.width = image.width;
						canvas.height = image.height;
						this.context.putImageData(image_data, 0, 0);
					},
				},
				main_image_list: _main_image_list,
				main_layer_list: _main_layer_list,
				image_lists: {
					lists: $('select.image-list').
						map(function(ind, elem) {
							return new html_types.ImageList($(elem));
						}).
						get(),
					push: function(element) {
						this.lists.push(new html_types.ImageList(element));
					},
				},
				layer_lists: {
					lists: $('select.layer-list').
						map(function(ind, elem) {
							return new html_types.LayerList($(elem));
						}).
						get(),
					push: function(element) {
						this.lists.push(new html_types.LayerList(element));
					},
				}
			};
		
		
		/* PUBLIC INTERFACE */
		
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
					comboCtors = {
						'image-list': html_types.ImageList,
						'layer-list': html_types.LayerList
					},
					comboContainers = {
						'image-list': html_objects.image_lists,
						'layer-list': html_objects.layer_lists
					},
					createCombo = function(cl) {
						return function(param) {
							var list = new comboCtors[cl](),
								elem = $(list.element).
									addClass(cl).
									addClass('wide-ctl');
							comboContainers[cl].push(elem);
							return elem;
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
			createForm: function(default_params) {
				var table = $(document.createElement('table')),
					createParamInput = this.createParamInput,
					self = this;
					
				default_params.forEach(function(param, i) {
					var input = self.createParamInput(param, 'param_'+i),
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

			onImageLoaded: function(image) {
				// An image has been loaded.
				// - add the image to all image lists
				// - select loaded image on the main image list
				
				html_objects.image_lists.lists.forEach(function(list) {
					list.add(image);
				});
				html_objects.main_image_list.select(image.list_item_id);
			},
			
			selectImage: function(image_id) {
				var image_list = $('#main_image_list'),
					option = image_list.
							 children('option[' + resource_id_key + '=' + image_id + ']').
							 attr('selected', 'selected').
							 change();
			},
			selectLastImage: function() {
				var image_list = $('#main_image_list'),
					option = image_list.
							 children().
							 last().
							 attr('selected', 'selected').
							 change();
			},
			
			drawImageOnCanvas: function(image) {
				var ctx = $('#input_preview_canvas')[0].getContext('2d'),
					image_data = image.getRGBA();
				ctx.canvas.width = image_data.width;
				ctx.canvas.height = image_data.height;
				ctx.putImageData(image_data, 0, 0);
			},
			getCanvasContext: function() {
				return $('#input_preview_canvas')[0].getContext('2d');
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
					self.image_structure_reflection.onImageLoaded(image);
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
			$('#input_parameters').empty().append(
				C2D.surface.param_form_mgr.createForm(method.params.in)
				);
			$('#output_parameters').empty().append(
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
			if(image) {
				self.image_structure_reflection.populateLayerLists
			}
				//self.image_structure_reflection.drawImageOnCanvas(image);
		};
		
		// user wants to launch an algorithm
		this.onRunMethod = function() {
			var option = $('#available_methods option:selected'), 
				method_id = option.attr(resource_id_key),
				method_params = C2D.client.getMethod(method_id).params.in,
				
				input_image_ctls = $('#input_parameters select.image-list'),
				selected_image_ids = input_image_ctls.map(function() {
					var list, image, id;
					list = this.list_object;
					image = list.getSelected();
					id = image.id;
					return id;
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
			// TODO: indicate calculation success
			console.log('Calculation succeeded:', results);
			try {
				var diff = C2D.client.digestResults(results);
				diff.new_images.forEach(function(new_image) {
					html_objects.image_lists.lists.forEach(function(list) {
						list.add(new_image);
					});
				});
				if(diff.new_images.length > 0)
					html_objects.main_image_list.select(diff.new_images[0].list_item_id);
			} catch(e) {
				// TODO: indicate indigestion
				console.error('Cannot digest results!\nReason:', e.message, '\nCall stack:', e.stack);
			}
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
		html_objects.main_image_list.element.change(function() {
			var image = html_objects.main_image_list.getSelected();
			html_objects.main_layer_list.populate(image);
			//html_objects.main_canvas.drawImage(image);
		});
		html_objects.main_layer_list.element.change(function() {
			var ctx = html_objects.main_canvas.context,
				layer_item = html_objects.main_layer_list.getSelectedItem();
			if(ctx && layer_item)
				layer_item.draw(ctx);
			else
				console.log('Cannot draw list item', layer_item, 'on context', ctx, '.');
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
		
		
		
		delete this.init;
	}
};


// the only on-load hook
$(document).ready(function() {
	C2D.surface.init();
});