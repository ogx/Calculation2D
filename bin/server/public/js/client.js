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
	runCalculationMethod: function(method, callback) {
		$.ajax({
			url: '/run',
			type: 'post',
			data: {
				method: method,
				images: []
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
	newImage: function(image_data) {
		this.image_structure.push({
			image: this.extractImage(image_data),
			data_layers: []
		});
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
			red: r,
			green: g,
			blue: b
		};
	},
	flattenImage: function(image, ctx) { // {w,h,r,g,b} -> ImageData
		var image_data = ctx.createImageData(image.width, image.height),
			output = image_data.data,
			r = image.red,
			g = image.green,
			b = image.blue;
		for(var i=0, j=0, n=output.length/4; i<n; i++) {
			output[j++] = r[i]*255;
			output[j++] = g[i]*255;
			output[j++] = b[i]*255;
			output[j++] = 255;
		}
		return image_data;
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
					ctx.getImageData(0, 0, canvas.clientWidth, canvas.clientHeight)
					);
			}
			img.src = data;
		};
		fr.readAsDataURL(file);
	});
	$('#input_preview').dblclick(function() {
		$('#input_path').trigger('click');
	});
	
	
	// plug in server's API
	$('#get_methods').click(function() {
		c2d_server.getCalculationMethods(function(methods){
			c2d_client.setMethods(methods);
			
			// populate combo
			methods_lookup = {};
			var combo = $('#available_methods');
			combo.empty();
			methods.forEach(function(method) {
				var option = $(document.createElement('option')),
					method_text = method.author + ': ' + method.name;
				option.text(method_text);
				option.attr('method_id', method.id);
				methods_lookup[method.id] = method;
				combo.append(option);
			});
			
			// handle detailed view
			combo.change(function() {
				var option = $('#available_methods option:selected'),
					method_id = option.attr('method_id'),
					method = methods_lookup[method_id];
				if(!method) {
					$('#method_details').hide();
					throw new Exception('Selected method not found!');
				}
				var max_params_length = 150;
				$('#method_details #name').text(method.name);
				$('#method_details #author').text(method.author);
				$('#method_details #description').text(method.description);
				$('#method_details #input_parameters').text(
					method.params.in.length > max_params_length ? 
					method.params.in.slice(0, max_params_length)+'...' : 
					method.params.in);
				$('#method_details #output_parameters').text(
					method.params.out.length > max_params_length ? 
					method.params.out.slice(0, max_params_length)+'...' : 
					method.params.out);
				$('#method_details').show();
			});
			combo.trigger('change');
		});
	});
	$('#run_method').click(function() {
		c2d_server.runCalculationMethod('blur', function(res){
			console.log(res)
		});
	});
	$('#get_status').click(function() {
		c2d_server.getStatus(function(res){
			console.log(res);
		});
	});
	$('#test').click(function() {
		var image = c2d_client.image_structure[c2d_client.image_structure.length-1].image,
			ch = image.blue;
		for(var i=0; i<ch.length; i++)
			ch[i] /= 2;
		var ctx = $('canvas')[0].getContext('2d');
			image_data = c2d_client.flattenImage(image, ctx);
		ctx.putImageData(image_data, 0, 0);
	});
	$('#load_btn').click(function() {
		$('#input_path').trigger('click');
	});
});