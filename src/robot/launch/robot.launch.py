from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    robot_xacro = PathJoinSubstitution([
        FindPackageShare("robot"),
        "urdf",
        "robot.urdf.xacro",
    ])

    rviz_config = PathJoinSubstitution([
        FindPackageShare("robot"),
        "rviz",
        "nav_demo.rviz",
    ])

    bridge_config = PathJoinSubstitution([
        FindPackageShare("robot"),
        "config",
        "bridge_parameters.yaml",
    ])

    obstacle_world = PathJoinSubstitution([
        FindPackageShare("world"),
        "worlds",
        "obstacle_world.sdf",
    ])

    robot_description = ParameterValue(
        Command(["xacro ", robot_xacro]),
        value_type=str,
    )

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([
                FindPackageShare("ros_gz_sim"),
                "launch",
                "gz_sim.launch.py",
            ])
        ]),
        launch_arguments={
            "gz_args": ["-r ", obstacle_world],
        }.items(),
    )

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[
            {
                "robot_description": robot_description,
                "use_sim_time": True,
            }
        ],
        output="screen",
    )

    joint_state_publisher = Node(
        package="joint_state_publisher",
        executable="joint_state_publisher",
        parameters=[
            {
                "use_sim_time": True,
            }
        ],
        output="screen",
    )

    spawn_robot = Node(
        package="ros_gz_sim",
        executable="create",
        arguments=[
            "-topic", "robot_description",
            "-name", "robot",
            "-x", "0.0",
            "-y", "0.0",
            "-z", "0.15",
        ],
        output="screen",
    )

    bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        parameters=[
            {
                "config_file": ParameterValue(bridge_config, value_type=str),
            }
        ],
        output="screen",
    )

    lidar_static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        arguments=[
            "--x", "0",
            "--y", "0",
            "--z", "0",
            "--roll", "0",
            "--pitch", "0",
            "--yaw", "0",
            "--frame-id", "lidar_link",
            "--child-frame-id", "robot/base_footprint/gpu_lidar",
        ],
        output="screen",
    )

    rviz = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", rviz_config],
        parameters=[
            {
                "use_sim_time": True,
            }
        ],
        output="screen",
    )

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        joint_state_publisher,
        lidar_static_tf,

        TimerAction(
            period=2.0,
            actions=[spawn_robot],
        ),

        bridge,
        rviz,
    ])