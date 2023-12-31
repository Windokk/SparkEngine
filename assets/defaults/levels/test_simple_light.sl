{
	"shaders": [
		{
			"defaultShader": [
				{ "vert": "./srcs/Shaders/Defaults/Engine/default.vert" },
				{ "frag": "./srcs/Shaders/Defaults/Engine/default.frag" },
				{ "geom": "" }
			]
		},
		{
			"skyboxShader": [
				{ "vert": "./srcs/Shaders/Defaults/Engine/Skyboxes/skybox.vert" },
				{ "frag": "./srcs/Shaders/Defaults/Engine/Skyboxes/skybox.frag" },
				{ "geom": "" }
			]
		},
		{
			"framebufferShader": [
				{ "vert": "./srcs/Shaders/Defaults/Engine/Framebuffer/framebuffer.vert" },
				{ "frag": "./srcs/Shaders/Defaults/Engine/Framebuffer/framebuffer.frag" },
				{ "geom": "" }
			]
		}

	],
	"skybox": [
		{ "shader": "skyboxShader" },
		{ "right": "./assets/defaults/textures/skyboxes/bluesky/right.jpg" },
		{ "left": "./assets/defaults/textures/skyboxes/bluesky/left.jpg" },
		{ "top": "./assets/defaults/textures/skyboxes/bluesky/top.jpg" },
		{ "bottom": "./assets/defaults/textures/skyboxes/bluesky/bottom.jpg" },
		{ "front": "./assets/defaults/textures/skyboxes/bluesky/front.jpg" },
		{ "back": "./assets/defaults/textures/skyboxes/bluesky/back.jpg" }
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
						{ "path": "./assets/defaults/models/plane/scene.gltf" },
						{ "shader": "defaultShader" },
						{ "instancing": "1" }
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
		},
		{
			"light2": [
				{
					"transform": [
						{ "location": [ "10.0", "10.0", "0.0" ] },
						{ "rotation": [ "1.0", "0.0", "0.0", "0.0" ] },
						{ "scale": [ "1.0", "1.0", "1.0" ] }
					]
				},
				{
					"light": [
						{ "type": "point" },
						{ "ambient": [ "0.05", "0.05", "0.05" ] },
						{ "diffuse": [ "0.8", "0.8", "0.8" ] },
						{ "specular": [ "1.0", "1.0", "1.0" ] },
						{ "constant": "1.0" },
						{ "linear": "0.09" },
						{ "quadratic": "0.032" },
						{ "intensity": "10" },
						{ "color": [ "1.0", "0.0", "0.0" ] }
					]
				}
			]
		},
		{
			"light3": [
				{
					"transform": [
						{
							"location": [ "-10.0", "10.0", "20.0" ]
						},
						{ "rotation": [ "1.0", "0.0", "0.0", "0.0" ] },
						{ "scale": [ "1.0", "1.0", "1.0" ] }
					]
				},
				{
					"light": [
						{ "type": "spot" },
						{ "direction": [ "0", "-1.0", "0" ] },
						{ "ambient": [ "0.05", "0.05", "0.05" ] },
						{ "diffuse": [ "0.8", "0.8", "0.8" ] },
						{ "specular": [ "1.0", "1.0", "1.0" ] },
						{ "constant": "1.0" },
						{ "linear": "0.09" },
						{ "quadratic": "0.032" },
						{ "cutOff": "12.5" },
						{ "outerCutOff": "15.0" },
						{ "intensity": "10" },
						{ "color": [ "0.0", "0.0", "10.0" ] }
					]
				}
			]
		}
	]
}