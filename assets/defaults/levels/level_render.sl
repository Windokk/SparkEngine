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
    { "right": "./assets/defaults/textures/skyboxes/base/blue.png" },
    { "left": "./assets/defaults/textures/skyboxes/base/blue.png" },
    { "top": "./assets/defaults/textures/skyboxes/base/blue.png" },
    { "bottom": "./assets/defaults/textures/skyboxes/base/blue.png" },
    { "front": "./assets/defaults/textures/skyboxes/base/blue.png" },
    { "back": "./assets/defaults/textures/skyboxes/base/blue.png" }
  ],
  "objects": [
    {
      "flying_island": [
        {
          "transform": [
            { "location": [ "0.0", "3.0", "0.0" ] },
            { "rotation": [ "1.0", "0.0", "0.0", "0.0" ] },
            { "scale": [ "10.0", "10.0", "10.0" ] }
          ]
        },
        {
          "model": [
            { "name": "flying_island" },
            { "path": "./assets/defaults/models/flying_island/scene.gltf" },
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
    }
  ]

}
