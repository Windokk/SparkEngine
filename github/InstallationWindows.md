## Build and Run Locally (Windows)

Clone the project

```bash
  git clone https://github.com/Windokk/SparkEngine
```

Go to the project's scripts directory

```bash
  cd SparkEngine/Scripts
```

Run the setup file

```bash
  Setup-Windows.bat
```

_______________________________________


> [!WARNING]
> The engine editor uses Qt6. To run it, you'll need to add an environment variable so that Qt can find the plugins directory.

### With cmd

```bash
setx QT_QPA_PLATFORM_PLUGIN_PATH "The absolute path to SparkEngine-Core\Source\Third-Party\plugins\Qt"
```

Example :

```bash
setx QT_QPA_PLATFORM_PLUGIN_PATH "C:\Users\test_user\Documents\GitHub\SparkEngine\SparkEngine-Core\Source\Third-Party\plugins\Qt"
```

### With Windows Gui

Search for "Edit the system environment variables" Windows searchbar

<img src="https://github.com/Windokk/SparkEngine/blob/master/github/env_search.png" width="500"/>

Click on "Environment Variables..."

<img src="https://github.com/Windokk/SparkEngine/blob/master/github/system_properties.png" width="500"/>

Click on "New" under "User variables"

Set Variable name to : QT_QPA_PLATFORM_PLUGIN_PATH

Set Variable value to : The absolute path to SparkEngine-Core\Source\Third-Party\plugins\Qt

> [!NOTE]
> In both case : Restart your computer to apply the new environment variable.

_______________________________________

You can now open the .sln file in "Spark Engine" folder

If you want to do it from the command line : go back to Spark Engine folder

```bash
  cd ../
```

Open the .sln file

```bash
  "Spark Engine.sln"
```
