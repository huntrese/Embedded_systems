import matplotlib.pyplot as plt
import random
import numpy as np

# Data based on your requirements list
moscow_data = {
    "Must Have": ['SR-01', 'SR-02', 'SR-03', 'SR-10', 'SR-12', 'SR-13', 'SR-14', 'SR-15', 'SR-16', 'SR-19', 'SR-21', 'SR-22', 'SR-23', 'SR-24', 'SR-26', 'SR-27', 'SR-28', 'SR-29', 'SR-32', 'SR-33', 'SR-35', 'SR-36', 'SR-37', 'SR-40', 'SR-41', 'SR-42', 'SR-43', 'SR-46', 'SR-49', 'SR-50'],
    "Should Have": ['SR-04', 'SR-05', 'SR-07', 'SR-08', 'SR-09', 'SR-17', 'SR-18', 'SR-20', 'SR-25', 'SR-30', 'SR-31', 'SR-34', 'SR-38', 'SR-39', 'SR-44', 'SR-45', 'SR-47', 'SR-48'],
    "Could Have": ['SR-06', 'SR-11']
}

# Define category centers on X-axis and colors
categories = list(moscow_data.keys())
category_centers = {cat: i for i, cat in enumerate(categories)}
region_colors = {
    'Must Have': (1.0, 0.0, 0.0, 0.1),   # Light Red (RGBA)
    'Should Have': (1.0, 0.65, 0.0, 0.1), # Light Orange (RGBA)
    'Could Have': (0.0, 0.5, 0.0, 0.1)    # Light Green (RGBA)
}
point_colors = {
    'Must Have': 'red',
    'Should Have': 'orange',
    'Could Have': 'green'
}

# Prepare data for plotting
plot_data = []
max_count = max(len(reqs) for reqs in moscow_data.values())  # Find max items in a category for Y-scaling

for category, req_ids in moscow_data.items():
    center_x = category_centers[category]
    for req_id in req_ids:
        # Add random jitter for scattering:
        scatter_x = center_x + random.uniform(-0.4, 0.4)  # Adjusted range for better spacing
        scatter_y = random.uniform(0, max_count)  # Keep within valid Y-range
        plot_data.append({'x': scatter_x, 'y': scatter_y, 'label': req_id, 'category': category})

# Create the plot
fig, ax = plt.subplots(figsize=(14, 9))  # Adjust figure size if needed

# Add colored background regions for each category
for category, center_x in category_centers.items():
    ax.axvspan(center_x - 0.5, center_x + 0.5, color=region_colors[category], zorder=0)  # Background layer

# Plot points with category-specific colors
for category, color in point_colors.items():
    cat_points = [p for p in plot_data if p['category'] == category]
    ax.scatter([p['x'] for p in cat_points], [p['y'] for p in cat_points],
               color=color, label=category, alpha=0.7, edgecolors='black', s=60)  # Changed edge color

# Add labels for each point
for p in plot_data:
    ax.text(p['x'] + 0.02, p['y'], p['label'], fontsize=7, verticalalignment='center')

# Configure axes and title
ax.set_xticks(list(category_centers.values()))
ax.set_xticklabels([f"{cat}\n(n={len(moscow_data[cat])})" for cat in categories])
ax.set_yticks([])  # Hide Y-axis ticks as they are just for spacing
ax.set_xlabel("MoSCoW Priority Category & Count")
ax.set_ylabel("Requirements (Scattered Vertically)")
ax.set_title("MoSCoW Requirement Distribution Map (Scattered)")

# Set plot limits (X based on category centers, Y based on scatter range)
ax.set_xlim(-0.6, len(categories) - 0.4)
ax.set_ylim(-1, max_count + 1)  # Add padding

ax.grid(axis='x', linestyle='--', alpha=0.5)  # Grid lines between categories

plt.tight_layout()  # Adjust layout
plt.show()
