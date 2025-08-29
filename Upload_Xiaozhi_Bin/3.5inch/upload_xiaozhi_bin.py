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

def get_module_version(module_name):
    """Get the installed version of a module"""
    try:
        result = subprocess.check_output([sys.executable, "-m", "pip", "show", module_name], stderr=subprocess.STDOUT)
        for line in result.decode().split('\n'):
            if line.startswith('Version:'):
                return line.split(':')[1].strip()
        return None
    except subprocess.CalledProcessError:
        return None

def install_module(module_name, version=None):
    """Install the module with optional version specification"""
    if version:
        module_spec = f"{module_name}=={version}"
        print(f"{module_name} version {version} is not installed. Installing now...")
    else:
        module_spec = module_name
        print(f"{module_name} is not installed. Installing now...")
    
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", module_spec])
        if version:
            print(f"{module_name} version {version} installation completed.")
        else:
            print(f"{module_name} installation completed.")
    except Exception as e:
        print(f"Error occurred while installing {module_name}: {e}")
        sys.exit(1)

def uninstall_module(module_name):
    """Uninstall the module"""
    print(f"Uninstalling {module_name}...")
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "uninstall", module_name, "-y"])
        print(f"{module_name} uninstalled successfully.")
    except Exception as e:
        print(f"Error occurred while uninstalling {module_name}: {e}")
        sys.exit(1)

def run_esptool_command():
    """Execute the esptool command"""
    command = [
        sys.executable, "-m", "esptool",
        "--chip", "esp32s3",
        #"--port", "COMx",
        "--baud", "2000000",
        "--before", "default-reset",
        "--after", "hard-reset",
        "write-flash",
        "-z",
        "--flash-mode", "keep",
        "--flash-freq", "keep",
        "--flash-size", "keep",
        "0x0", "bin/bootloader.bin",
        "0x100000", "bin/xiaozhi.bin",
        "0x8000", "bin/partition-table.bin",
        "0xd000", "bin/ota_data_initial.bin",
        "0x10000", "bin/srmodels.bin"
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
    required_version = "5.0.1"
    
    # Check if esptool is installed
    if not is_module_installed(module_name):
        install_module(module_name, required_version)
    else:
        # Check if the installed version matches the required version
        current_version = get_module_version(module_name)
        if current_version != required_version:
            print(f"Installed version {current_version} does not match required version {required_version}")
            uninstall_module(module_name)
            install_module(module_name, required_version)
        else:
            print(f"{module_name} version {required_version} is already installed.")

    # Execute esptool command
    run_esptool_command()
    
if __name__ == "__main__":
    main()