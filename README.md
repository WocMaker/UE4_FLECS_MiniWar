# FLECS Embedded in Unreal Engine Demo
![image](https://github.com/user-attachments/assets/db83a1ee-b292-4b80-8197-22bb4fb288f4)

## Overview
- **Unreal Engine Version**: 4.26
- **Scene Description**: The scenario features continuous troop deployment from both sides, with defensive towers that target and attack soldiers within their laser range.


## Key Features
1. **Troop Deployment**: Soldiers continuously spawn from both ends of the battlefield.
![image](https://github.com/user-attachments/assets/4aaa8cb9-c9ae-4ffb-88d5-78c11b14020a)
3. **Defensive Towers**: Towers are equipped with lasers to engage and attack soldiers that enter their line of fire.
![image](https://github.com/user-attachments/assets/03f4fc07-7d59-47dd-976a-96fa227a8cd5)


5. **Spaceship Engagement**: Laser attacks on spacecraft cause damage. When a spacecraft's health reaches zero, it is destroyed.
6. **ECS Optimization Toggle**: Utilize the ECS Switch within the Blueprint to choose whether to employ ECS for optimization.
![1725615890765](https://github.com/user-attachments/assets/5a6421a4-6c2d-4845-8b8f-5780f6205153)

## Performance Results
- **Platform**: On integrated graphics, idle framerate is approximately 70fps.
- **Comparison Sence**: full combat with 6,000 to 8,000 entities(actors), and lasers
- **Framerate Comparison**:
  - Traditional Development: <10fps
  - With ECS: ~30-40fps
