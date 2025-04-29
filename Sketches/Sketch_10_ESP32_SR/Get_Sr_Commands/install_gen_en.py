import importlib.util
import subprocess
import sys

def check_and_install(package_name):
    # Check if the package is already installed
    package_spec = importlib.util.find_spec(package_name)
    if package_spec is None:
        print(f"{package_name} is not installed, installing now...")
        # Install the package using pip
        subprocess.check_call([sys.executable, "-m", "pip", "install", package_name])
        print(f"{package_name} installation completed")
    else:
        print(f"{package_name} is already installed")

# Check and install g2p_en
check_and_install("g2p_en")