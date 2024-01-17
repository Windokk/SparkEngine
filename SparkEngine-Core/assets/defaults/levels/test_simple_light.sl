{
	"shaders": [
    {
      "defaultShader": [
        { "vert": "../SparkEngine-Core/assets/defaults/shaders/default.vert" },
        { "frag": "../SparkEngine-Core/assets/defaults/shaders/default.frag" },
        { "geom": "" }
      ]
    },
    {
      "skyboxShader": [
        { "vert": "../SparkEngine-Core/assets/defaults/shaders/Skyboxes/skybox.vert" },
        { "frag": "../SparkEngine-Core/assets/defaults/shaders/Skyboxes/skybox.frag" },
        { "geom": "" }
      ]
    },
    {
      "framebufferShader": [
        { "vert": "../SparkEngine-Core/assets/defaults/shaders/Framebuffer/framebuffer.vert" },
        { "frag": "../SparkEngine-Core/assets/defaults/shaders/Framebuffer/framebuffer.frag" },
        { "geom": "" }
      ]
    }

  ],
	"skybox": [
		{ "shader": "skyboxShader" },
		{ "right": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/right.jpg" },
		{ "left": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/left.jpg" },
		{ "top": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/top.jpg" },
		{ "bottom": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/bottom.jpg" },
		{ "front": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/front.jpg" },
		{ "back": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/back.jpg" }
	],
	"objects": [
		{
			"floor": [
				{
					"transform": [
						{ "location": [ "0.0", "3.0", "0.0" ] },
						{ "rotation": [ "1.0", "0.0", "0.0", "0.0" ] },
						{ "scale": [ "50.0", "1.0", "50.0" ] }
					]
				},
				{
					"model": [
						{ "name": "floor" },
						{ "path": "../SparkEngine-Core/assets/defaults/models/plane/scene.gltf" },
						{ "shader": "defaultShader" },
						{ "instancing": "1" }
					]
				},
				{
					"rigidbody": [
					  { "mass": "0" },
					  { "collider": "plane"}
					]
				},
				{
					"plane_collider": [
					  { "distance": "10" },
					  { "normal": [ "0.0", "1.0", "0.0" ] }
					]
				}
			]
		},
		{
			"ball": [
				{
					"transform": [
						{ "location": [ "0.0", "10.0", "0.0" ] },
						{ "rotation": [ "1.0", "0.0", "0.0", "0.0" ] },
						{ "scale": [ "5.0", "5.0", "5.0" ] }
					]
				},
				{
					"model": [
						{ "name": "ball" },
						{ "path": "../SparkEngine-Core/assets/defaults/models/ball/scene.gltf" },
						{ "shader": "defaultShader" },
						{ "instancing": "1" }
					]
				},
				{
					"rigidbody": [
					  { "mass": "1" },
					  { "collider": "sphere"}
					]
				},
				{
					"sphere_collider": [
					  { "radius": "10" },
					  { "center": [ "0.0", "0.0", "0.0" ] }
					]
				}
			]
		},
		{
			"light1": [
				{
					"transform": [
						{ "location": [ "0.0", "3.0", "0.0" ] },
						{ "rotation": [ "1.0", "0.0", "0.0", "0.0" ] },
						{ "scale": [ "1.0", "1.0", "1.0" ] }
					]
				},
				{
					"light": [
						{ "type": "directionnal" },
						{ "direction": [ "-0.2", "-1.0", "-0.3" ] },
						{ "ambient": [ "0.05", "0.05", "0.05" ] },
						{ "diffuse": [ "0.8", "0.8", "0.8" ] },
						{ "specular": [ "0.5", "0.5", "0.5" ] },
						{ "intensity": "1.0" },
						{ "color": [ "1.0", "1.0", "1.0" ] }
					]
				}
			]
		}
	]
}