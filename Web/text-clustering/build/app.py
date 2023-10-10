import os
import zipfile
from os.path import join as pathjoin
from flask import Flask, render_template, jsonify, request

import ai

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

# 上传训练数据并训练模型
@app.route('/train', methods=['POST'])
def train():
    # 获取模型名称和样本数量
    model_name = request.args.get('model')
    sample_count = int(request.args.get('sampleCount'))

    if not isinstance(model_name, str) or '..' in model_name or '/' in model_name:
        return "Bad model name!!!"

    upload_dir = f'uploads/{model_name}'
    os.makedirs(upload_dir, exist_ok=True)

    # 保存每个上传的样本
    for i in range(sample_count):
        sample_file = request.files.get(f'sample_{i}')
        if sample_file:
            sample_file.save(pathjoin(upload_dir, f'sample_{i}.zip'))

    # 解压上传的压缩包
    for i in range(sample_count):
        sample_type = request.form.get(f'sample_{i}_type')
        if '..' in sample_type:
            return "Bad sample type"

        zip_file_path = pathjoin(upload_dir, f'sample_{i}.zip')
        extract_dir = pathjoin(upload_dir, sample_type)
        
        if os.path.exists(zip_file_path):
            with zipfile.ZipFile(zip_file_path, 'r') as zip_ref:
                for zip_file_info in zip_ref.infolist():
                    zip_ref.extract(zip_file_info, extract_dir)
            os.remove(zip_file_path)

    ai.train(upload_dir)
    return jsonify({"message": "模型训练成功"})

    

# 使用保存的模型进行分类
@app.route('/classify', methods=['POST'])
def classify_text():
    model_name = request.args.get('modelName')
    text = request.json['text']
    print(text)

    return jsonify({"result": ai.classify(model_name, text)})

@app.route('/getModels', methods=['GET'])
def get_models():
    return jsonify(os.listdir(ai.MODEL_DIR))

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=3000)
