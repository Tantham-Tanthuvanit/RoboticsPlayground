Import("env")
import os

libs_pkg = env.PioPlatform().get_package_dir("framework-arduinoespressif32-esp32-libs")

if libs_pkg:
    flags_file = os.path.join(libs_pkg, "flags", "includes")
    base_inc = os.path.join(libs_pkg, "include")
    
    if os.path.exists(flags_file):
        with open(flags_file, "r") as f:
            content = f.read()
        
        paths = []
        for token in content.split():
            if token.startswith("-iwithprefixbefore"):
                rel_path = token.replace("-iwithprefixbefore", "").strip()
                full_path = os.path.normpath(os.path.join(base_inc, rel_path))
                if os.path.exists(full_path):
                    paths.append(full_path)
            elif token.startswith("-I"):
                rel_path = token[2:].strip()
                full_path = os.path.normpath(os.path.join(base_inc, rel_path))
                if os.path.exists(full_path):
                    paths.append(full_path)
        
        env.Append(CPPPATH=paths)