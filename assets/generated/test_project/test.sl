{
    "objects": [
        {
            "flying_island": [
                {
                    "transform": [
                        {
                            "location": [
                                "0.000000",
                                "3.000000",
                                "0.000000"
                            ]
                        },
                        {
                            "rotation": [
                                "1.000000",
                                "0.000000",
                                "0.000000",
                                "0.000000"
                            ]
                        },
                        {
                            "scale": [
                                "10.000000",
                                "10.000000",
                                "10.000000"
                            ]
                        }
                    ]
                },
                {
                    "model": [
                        {
                            "name": "flying_island"
                        },
                        {
                            "path": "./assets/defaults/models/flying_island/scene.gltf"
                        },
                        {
                            "shader": "defaultShader"
                        },
                        {
                            "instancing": "1"
                        }
                    ]
                }
            ]
        },
        {
            "light1": [
                {
                    "transform": [
                        {
                            "location": [
                                "0.000000",
                                "3.000000",
                                "0.000000"
                            ]
                        },
                        {
                            "rotation": [
                                "1.000000",
                                "0.000000",
                                "0.000000",
                                "0.000000"
                            ]
                        },
                        {
                            "scale": [
                                "1.000000",
                                "1.000000",
                                "1.000000"
                            ]
                        }
                    ]
                },
                {
                    "light": [
                        {
                            "type": "directionnal"
                        },
                        {
                            "direction": [
                                "-0.200000",
                                "-1.000000",
                                "-0.300000"
                            ]
                        },
                        {
                            "ambient": [
                                "0.050000",
                                "0.050000",
                                "0.050000"
                            ]
                        },
                        {
                            "diffuse": [
                                "0.800000",
                                "0.800000",
                                "0.800000"
                            ]
                        },
                        {
                            "specular": [
                                "0.500000",
                                "0.500000",
                                "0.500000"
                            ]
                        },
                        {
                            "intensity": "1.000000"
                        },
                        {
                            "color": [
                                "1.000000",
                                "1.000000",
                                "1.000000"
                            ]
                        }
                    ]
                }
            ]
        }
    ],
    "shaders": [
        {
            "defaultShader": [
                {
                    "vert": "./srcs/Shaders/Defaults/Engine/default.vert"
                },
                {
                    "frag": "./srcs/Shaders/Defaults/Engine/default.frag"
                },
                {
                    "geom": ""
                }
            ]
        },
        {
            "skyboxShader": [
                {
                    "vert": "./srcs/Shaders/Defaults/Engine/Skyboxes/skybox.vert"
                },
                {
                    "frag": "./srcs/Shaders/Defaults/Engine/Skyboxes/skybox.frag"
                },
                {
                    "geom": ""
                }
            ]
        },
        {
            "framebufferShader": [
                {
                    "vert": "./srcs/Shaders/Defaults/Engine/Framebuffer/framebuffer.vert"
                },
                {
                    "frag": "./srcs/Shaders/Defaults/Engine/Framebuffer/framebuffer.frag"
                },
                {
                    "geom": ""
                }
            ]
        }
    ],
    "skybox": [
        {
            "shader": "skyboxShader"
        },
        {
            "right": "./assets/defaults/textures/skyboxes/base/blue.png"
        },
        {
            "left": "./assets/defaults/textures/skyboxes/base/blue.png"
        },
        {
            "top": "./assets/defaults/textures/skyboxes/base/blue.png"
        },
        {
            "bottom": "./assets/defaults/textures/skyboxes/base/blue.png"
        },
        {
            "front": "./assets/defaults/textures/skyboxes/base/blue.png"
        },
        {
            "back": "./assets/defaults/textures/skyboxes/base/blue.png"
        }
    ]
}