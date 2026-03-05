#!/usr/bin/env python3
"""Preprocess a Markdown file: render mermaid code blocks as PNG images."""

import re
import subprocess
import sys
import os
import tempfile

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} INPUT.md OUTPUT.md")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]
    img_dir = os.path.join(os.path.dirname(output_path), "mermaid_imgs")
    os.makedirs(img_dir, exist_ok=True)

    with open(input_path, "r") as f:
        content = f.read()

    pattern = re.compile(r"```mermaid\n(.*?)```", re.DOTALL)
    counter = [0]

    def replace(match):
        diagram = match.group(1)
        counter[0] += 1
        img_path = os.path.join(img_dir, f"diagram_{counter[0]}.png")

        with tempfile.NamedTemporaryFile(suffix=".mmd", mode="w", delete=False) as tmp:
            tmp.write(diagram)
            tmp_path = tmp.name

        try:
            subprocess.run(
                ["npx", "--yes", "@mermaid-js/mermaid-cli", "-i", tmp_path, "-o", img_path],
                check=True,
                capture_output=True,
            )
        finally:
            os.unlink(tmp_path)

        rel_path = os.path.relpath(img_path, os.path.dirname(output_path))
        return f"![]({rel_path})"

    result = pattern.sub(replace, content)

    with open(output_path, "w") as f:
        f.write(result)

if __name__ == "__main__":
    main()
