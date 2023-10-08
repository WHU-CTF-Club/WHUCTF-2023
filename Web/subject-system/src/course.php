<?php
// 连接到数据库，请根据您的数据库配置进行修改
$servername = "127.0.0.1";
$username = "root";
$password = "root";
$database = "WHUsubject";

// 创建数据库连接
$conn = new mysqli($servername, $username, $password, $database);

// 检查连接是否成功
if ($conn->connect_error) {
    die("连接数据库失败: " . $conn->connect_error);
}

// 获取排序参数（默认为按课程名升序）
$sortOrder = isset($_GET['sortOrder']) ? $_GET['sortOrder'] : 'name';

// 构建SQL查询语句根据排序参数排序课程
$sql = "SELECT * FROM courses ORDER BY ".$sortOrder.";";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $courses = array();
    while ($row = $result->fetch_assoc()) {
        $courses[] = $row;
    }
    echo json_encode($courses);
} else {
    echo "没有找到课程数据";
}

// 关闭数据库连接
$conn->close();
?>
