// JavaScript代码
let availableCourses = [];
let selectedCourses = [];

// 函数用于获取课程数据
function fetchAndDisplayCourses() {
    const sortOrder = document.getElementById('sortOrder').value;

    // 发送请求到后端，携带排序参数
    fetch(`/course.php?sortOrder=${sortOrder}`)
        .then(response => response.json())
        .then(data => {
            availableCourses = data;
            updateAvailableCourses();
        })
        .catch(error => console.error('获取课程数据时出错：', error));
}

// 获取课程数据，初始时默认排序方式为按课程名升序
fetchAndDisplayCourses();

function selectCourse(courseId) {
    // 检查是否已选择相同的课程
    if (selectedCourses.some(course => course.id === courseId.toString())) {
        alert('您已选择该课程');
        return;
    }

    // 在这里处理课程选择逻辑
    // 可以添加选课逻辑，例如检查时间冲突等
    // 这里只是一个简单的示例，将选课的课程对象添加到已选课程列表中
    const selectedCourse = availableCourses.find(course => course.id == courseId);
    selectedCourses.push(selectedCourse);

    // 更新已选课程列表
    updateSelectedCourses();
}

function updateAvailableCourses() {
    const availableCoursesContainer = document.getElementById('available-courses');
    availableCoursesContainer.innerHTML = '';

    for (const course of availableCourses) {
        const courseElement = document.createElement('div');
        courseElement.className = 'course';
        courseElement.innerHTML = `
            <h3>${course.name}</h3>
            <p>${course.description}</p>
            <p><strong>学分：</strong>${course.credits}</p>
            <p><strong>所属专业</strong>${course.major}</p>
            <button onclick="selectCourse(${course.id})">选择课程</button>
        `;
        availableCoursesContainer.appendChild(courseElement);
    }
}

function updateSelectedCourses() {
    const selectedCoursesContainer = document.getElementById('selected-courses');
    selectedCoursesContainer.innerHTML = '';

    for (const course of selectedCourses) {
        const courseElement = document.createElement('div');
        courseElement.className = 'course';
        courseElement.innerHTML = `
            <h3>${course.name}</h3>
            <p>${course.description}</p>
            <button onclick="removeSelectedCourse(${course.id})">删除课程</button>
        `;
        selectedCoursesContainer.appendChild(courseElement);
    }
}

function removeSelectedCourse(courseId) {
    // 从已选课程中删除指定课程
    selectedCourses = selectedCourses.filter(course => course.id !== courseId.toString());

    // 更新已选课程列表
    updateSelectedCourses();
}