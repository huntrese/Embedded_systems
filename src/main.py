import requests
import base64
import json
import os

def direct_mermaid_png(
    mermaid_content,
    style="default",
    output_file=None
):
    """
    Generate Mermaid diagram directly using the Mermaid.ink API
    """
    # Style presets
    styles = {
        "default": {
            "theme": "",
            "bg_color": "white",
        },
        "dark": {
            "theme": "%%{init: {'theme': 'dark'}}%%\n",
            "bg_color": "#0F1525",
        },
        "corporate": {
            "theme": "%%{init: {'themeVariables': {'primaryColor': '#0072C6', 'primaryTextColor': '#fff', 'primaryBorderColor': '#005999', 'lineColor': '#555555'}}}%%\n",
            "bg_color": "#F0F8FF",
        },
        "vibrant": {
            "theme": "%%{init: {'themeVariables': {'primaryColor': '#FF5252', 'primaryTextColor': '#fff', 'primaryBorderColor': '#C50E0E', 'lineColor': '#FFAB40'}}}%%\n",
            "bg_color": "#FFFAF0",
        },
        "minimal": {
            "theme": "%%{init: {'themeVariables': {'primaryColor': '#F5F5F5', 'primaryTextColor': '#333', 'primaryBorderColor': '#DDD', 'lineColor': '#999'}}}%%\n",
            "bg_color": "white",
        }
    }
    
    # Get style config or default to "default"
    style_config = styles.get(style, styles["default"])
    
    # Combine theme with content
    full_mermaid = style_config["theme"] + mermaid_content
    
    # Encode the Mermaid syntax
    mermaid_encoded = base64.b64encode(full_mermaid.encode('utf-8')).decode('utf-8')
    
    # Get the image from mermaid.ink
    img_url = f"https://mermaid.ink/img/{mermaid_encoded}"
    response = requests.get(img_url)
    
    if response.status_code != 200:
        raise ValueError(f"Failed to get diagram. Status code: {response.status_code}")
    
    # Save to file if requested
    if output_file:
        with open(output_file, 'wb') as f:
            f.write(response.content)
    
    return response.content