{
    "objects": [
        {
            "floor": [
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
                                "50.000000",
                                "1.000000",
                                "50.000000"
                            ]
                        }
                    ]
                },
                {
                    "model": [
                        {
                            "name": "floor"
                        },
                        {
                            "path": "../SparkEngine-Core/assets/defaults/models/plane/scene.gltf"
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
            "ball": [
                {
                    "transform": [
                        {
                            "location": [
                                "0.000000",
                                "10.000000",
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
                                "5.000000",
                                "5.000000",
                                "5.000000"
                            ]
                        }
                    ]
                },
                {
                    "model": [
                        {
                            "name": "ball"
                        },
                        {
                            "path": "../SparkEngine-Core/assets/defaults/models/ball/scene.gltf"
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
                    "vert": "../SparkEngine-Core/assets/defaults/shaders/default.vert"
                },
                {
                    "frag": "../SparkEngine-Core/assets/defaults/shaders/default.frag"
                },
                {
                    "geom": ""
                }
            ]
        },
        {
            "skyboxShader": [
                {
                    "vert": "../SparkEngine-Core/assets/defaults/shaders/Skyboxes/skybox.vert"
                },
                {
                    "frag": "../SparkEngine-Core/assets/defaults/shaders/Skyboxes/skybox.frag"
                },
                {
                    "geom": ""
                }
            ]
        },
        {
            "framebufferShader": [
                {
                    "vert": "../SparkEngine-Core/assets/defaults/shaders/Framebuffer/framebuffer.vert"
                },
                {
                    "frag": "../SparkEngine-Core/assets/defaults/shaders/Framebuffer/framebuffer.frag"
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
            "right": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/right.jpg"
        },
        {
            "left": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/left.jpg"
        },
        {
            "top": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/top.jpg"
        },
        {
            "bottom": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/bottom.jpg"
        },
        {
            "front": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/front.jpg"
        },
        {
            "back": "../SparkEngine-Core/assets/defaults/textures/skyboxes/bluesky/back.jpg"
        }
    ]
}