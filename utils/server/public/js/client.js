/*
 *	This module manages client-side data. 
 *
 *	Interfaces: 
 *		- ...
 */

var C2D = C2D || {};


C2D.client = {
	init: function() {
		var image_structure = C2D._is = new C2D.ImageStructure(),
			methods_lookup = C2D._m = {};
		
		/* IMAGES MANAGEMENT */
		this.loadImage = function(img, name) {
			return image_structure.loadImage(img, name);
		};
		this.getImagesCount = function() {
			var num_images = 0;
			for(var id in image_structure.images)
				num_images++;
			return num_images;
		};
		this.forImages = function(callback) {
			for(var id in image_structure.images)
				callback(image_structure.images[id]);
		};
		this.findImage = function(id) {
			return image_structure.findImage(id);
		};
		this.wrapStructure = function(selected_image_ids) {
			return image_structure.wrapItUp(selected_image_ids);
		};
		this.digestResults = function(results) {
			return image_structure.digestResults(results.image_structure);
		};
		
		/* METHODS MANAGEMENT */
		this.addMethod = function(method) {
			methods_lookup[method.id] = method;
		};
		this.getMethod = function(id) {
			return methods_lookup[id];
		};
		
		delete this.init;
	}
};

