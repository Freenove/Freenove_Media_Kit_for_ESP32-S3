import subprocess
import sys
import os

def is_module_installed(module_name):
    """Check if the module is installed"""
    try:
        subprocess.check_output([sys.executable, "-m", "pip", "show", module_name], stderr=subprocess.STDOUT)
        return True
    except subprocess.CalledProcessError:
        return False

def install_module(module_name):
    """Install the module"""
    print(f"{module_name} is not installed. Installing now...")
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", module_name])
        print(f"{module_name} installation completed.")
    except Exception as e:
        print(f"Error occurred while installing {module_name}: {e}")
        sys.exit(1)

def run_esptool_command():
    """Execute the esptool command"""
    command = [
        sys.executable, "-m", "esptool",
        "--chip", "esp32s3",
        #"--port", "COM4",
        "--baud", "2000000",
        "--before", "default_reset",
        "--after", "hard_reset",
        "write_flash",
        "-z",
        "--flash_mode", "keep",
        "--flash_freq", "keep",
        "--flash_size", "keep",
        "0x510000", "srmodels.bin"
    ]
    try:
        print("Executing esptool command...")
        subprocess.check_call(command)
        print("esptool command executed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"esptool command execution failed: {e}")
        sys.exit(1)

def main():
    module_name = "esptool"
    
    # Check if esptool is installed
    if not is_module_installed(module_name):
        install_module(module_name)
    else:
        print(f"{module_name} is already installed.")

    # Execute esptool command
    run_esptool_command()

if __name__ == "__main__":
    main()