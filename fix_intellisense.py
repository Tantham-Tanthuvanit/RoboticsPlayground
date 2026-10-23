Import("env")
import os

board_mcu = env.BoardConfig().get("build.mcu", "esp32").lower()
platform = env.PioPlatform()

candidate_pkg_names = [
    f"framework-arduinoespressif32-{board_mcu}-libs",
    "framework-arduinoespressif32-esp32s3-libs",
    "framework-arduinoespressif32-esp32-libs",
]

libs_pkg = None
for name in candidate_pkg_names:
    pkg = platform.get_package_dir(name)
    if pkg and os.path.exists(pkg):
        libs_pkg = pkg
        break

if libs_pkg:
    flags_file = os.path.join(libs_pkg, "flags", "includes")
    base_inc = os.path.join(libs_pkg, "include")

    if os.path.exists(flags_file):
        with open(flags_file, "r") as f:
            tokens = f.read().split()

        paths = []
        i = 0
        while i < len(tokens):
            tok = tokens[i]
            if tok == "-iwithprefixbefore":
                i += 1
                if i < len(tokens):
                    rel = tokens[i]
                    p = os.path.normpath(os.path.join(base_inc, rel))
                    if os.path.exists(p):
                        paths.append(p)
            elif tok.startswith("-iwithprefixbefore"):
                rel = tok[len("-iwithprefixbefore"):].lstrip("=/")
                p = os.path.normpath(os.path.join(base_inc, rel))
                if os.path.exists(p):
                    paths.append(p)
            elif tok == "-I":
                i += 1
                if i < len(tokens):
                    rel = tokens[i]
                    p = os.path.normpath(os.path.join(base_inc, rel)) if not os.path.isabs(rel) else rel
                    if os.path.exists(p):
                        paths.append(p)
            elif tok.startswith("-I"):
                rel = tok[2:].lstrip()
                p = os.path.normpath(os.path.join(base_inc, rel)) if not os.path.isabs(rel) else rel
                if os.path.exists(p):
                    paths.append(p)
            i += 1

        seen = set()
        unique_paths = [p for p in paths if not (p in seen or seen.add(p))]
        env.Append(CPPPATH=unique_paths)
