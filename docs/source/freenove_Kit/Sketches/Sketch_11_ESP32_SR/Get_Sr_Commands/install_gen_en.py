import importlib.util
import subprocess
import sys
import os
import shutil

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

# Detect operating system platform
def get_os():
    platform = sys.platform
    if platform.startswith('linux'):
        return 'Linux'
    elif platform == 'darwin':
        return 'Mac'
    elif platform.startswith('win') or platform == 'cygwin':
        return 'Windows'
    else:
        return 'Unknown'
        exit(1) # Exit on error

# Check if nltk_data directory exists
def checkFileExists():
    if System == 'Windows': # For Windows system
        nltk_dir = os.path.join(os.environ['USERPROFILE'], 'nltk_data')
        isdir = os.path.isdir(nltk_dir)
        if isdir == True: # Check if nltk_data exists in user directory
            print("nltk_data already exists")
            shutil.rmtree(os.path.join(os.environ['USERPROFILE'], 'nltk_data'))
        else:    
            print("nltk_data not found")
    if System == 'Mac' or System == 'Linux': # For Windows system
        nltk_dir = os.path.join(os.environ['HOME'], 'nltk_data')
        isdir = os.path.isdir(nltk_dir)
        if isdir == True: # Check if nltk_data exists in user directory
            print("nltk_data already exists")
            shutil.rmtree(os.path.join(os.environ['HOME'], 'nltk_data'))
            return True
        else:    
            print("nltk_data not found")
            return False
    # Linux and Mac implementations would go here
    # (commented out in original code)

# File operations using shutil
def MoveFile():
    current_dir = os.path.join(os.getcwd(), 'nltk_data')                # Get current directory
    if System == 'Windows':                                             # If nltk_data doesn't exist
        user_dir = os.path.join(os.environ['USERPROFILE'], 'nltk_data') # Get user directory
        shutil.copytree(current_dir, user_dir)                          # Copy to user directory
        if os.path.isdir(user_dir):                                     # Verify copy success
            print("Directory moved successfully")
    if System == 'Mac' or System == 'Linux':                            # If nltk_data doesn't exist
        user_dir = os.path.join(os.environ['HOME'], 'nltk_data')        # Get user directory
        shutil.copytree(current_dir, user_dir)                          # Copy to user directory
        if os.path.isdir(user_dir):                                     # Verify copy success
            print("Directory moved successfully")

if __name__ == "__main__":
    System = get_os()                                    # Detect operating system
    print(f"The current operating system is: {System}")
    checkFileExists()                                    # Check if directory exists
    MoveFile()                                           # Copy if doesn't exist, skip if exists
    check_and_install("g2p_en")                          # Check and install g2p_en package