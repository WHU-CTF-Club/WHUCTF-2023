// 前端逻辑
document.addEventListener("DOMContentLoaded", function () {
    const addSampleButton = document.getElementById("addSample");
    const clearSamplesButton = document.getElementById("clearSamples");
    const sampleList = document.getElementById("sampleList");
    const trainButton = document.getElementById("trainButton");

    // 初始时只显示介绍信息
    const introPage = document.getElementById("intro");
    const trainPage = document.getElementById("train");
    const classifyPage = document.getElementById("classify");

    introPage.style.display = "block";
    trainPage.style.display = "none";
    classifyPage.style.display = "none";

    // 导航链接点击事件
    const trainLink = document.querySelector('a[href="#train"]');
    const classifyLink = document.querySelector('a[href="#classify"]');

    trainLink.addEventListener("click", function (e) {
        e.preventDefault();
        introPage.style.display = "none";
        trainPage.style.display = "block";
        classifyPage.style.display = "none";
        classificationResult.style.display = "none"; // 隐藏分类结果
    });

    classifyLink.addEventListener("click", function (e) {
        e.preventDefault();
        introPage.style.display = "none";
        trainPage.style.display = "none";
        classifyPage.style.display = "block";
        classificationResult.style.display = "none"; // 隐藏分类结果
        loadModels();
    });

    // 添加上传样本点
    let sampleCount = 1;

    addSampleButton.addEventListener("click", function () {
        const sampleDiv = document.createElement("div");
        sampleDiv.classList.add("form-row", "mb-3");
        
        const sampleInput = document.createElement("input");
        sampleInput.type = "file";
        sampleInput.accept = ".zip";
        sampleInput.classList.add("col");
        sampleInput.name = `sample_${sampleCount}`;
        sampleInput.id = `sample_${sampleCount}`;
        
        const sampleType = document.createElement("input");
        sampleType.type = "text";
        sampleType.classList.add("col");
        sampleType.classList.add("form-control");
        sampleType.placeholder = "输入类型名称";
        
        sampleDiv.appendChild(sampleType);
        sampleDiv.appendChild(sampleInput);
        
        sampleList.appendChild(sampleDiv);
        
        sampleCount++;
    });


    // 清除所有上传点
    clearSamplesButton.addEventListener("click", function () {
        sampleList.innerHTML = ""; // 清除所有上传点和相关类型名称输入框
        sampleCount = 0;
    });

    // 训练模型
    trainButton.addEventListener("click", async () => {
        const modelNameInput = document.getElementById("modelName");

        const sampleInputs = document.querySelectorAll('[type="file"]');
        const sampleTypes = document.querySelectorAll('[type="text"]');
        const formData = new FormData();

        for (let i = 0; i < sampleInputs.length; i++) {
            const sampleInput = sampleInputs[i];
            const sampleType = sampleTypes[i];
            
            if (sampleInput.files.length > 0) {
                const sampleFile = sampleInput.files[0];
                formData.append(`sample_${i}`, sampleFile);
                
                // 将类型名称作为GET参数
                formData.append(`sample_${i}_type`, sampleType.value);
            }
        }

        try {
            // 发送训练数据到后端进行训练
            const response = await fetch(`/train?model=${modelNameInput.value}&sampleCount=${sampleCount}`, {
                method: "POST",
                body: formData,
            });

            if (response.ok) {
                trainedModel = await response.json();
                alert(trainedModel);
            } else {
                alert("训练失败");
            }
        } catch (error) {
            console.error(error);
        }
    });

    // 获取模型选择框和待分类文本输入框
    const modelSelect = document.getElementById("modelSelect");
    const classifyText = document.getElementById("classifyText");

    const classifyButton = document.getElementById("classifyButton");

    // 加载所有模型选项
    async function loadModels() {
        try {
            modelSelect.innerHTML = "";
            const response = await fetch("/getModels");
            if (response.ok) {
                const models = await response.json();
                models.forEach((model) => {
                    const option = document.createElement("option");
                    option.value = model;
                    option.textContent = model;
                    modelSelect.appendChild(option);
                });
            } else {
                console.error("无法加载模型列表");
            }
        } catch (error) {
            console.error(error);
        }
    }

    // 在页面加载时加载模型列表
    window.addEventListener("DOMContentLoaded", loadModels);

    // 分类文本
    classifyButton.addEventListener("click", async () => {
        const selectedModel = modelSelect.value;
        const textToClassify = classifyText.value;

        try {
            // 发送分类请求
            const response = await fetch(`/classify?modelName=${selectedModel}`, {
                method: "POST",
                body: JSON.stringify({ text: textToClassify }),
                headers: {
                    "Content-Type": "application/json",
                },
            });

            if (response.ok) {
                const result = await response.json();
                alert(`分类结果: ${result}`);
            } else {
                alert("分类失败");
            }
        } catch (error) {
            console.error(error);
        }
    });

});
