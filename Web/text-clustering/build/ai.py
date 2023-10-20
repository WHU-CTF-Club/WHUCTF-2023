import os
import pickle
import numpy as np
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.cluster import KMeans

MODEL_DIR = os.path.join(os.path.dirname(__file__), "models")
if not os.path.exists(MODEL_DIR):
    os.makedirs(MODEL_DIR)

def readtext_from_dir(dirname : str) -> list:
    text_list = []

    all_file = os.listdir(dirname)
    for file in all_file:
        filepath = os.path.join(dirname, file)
        if os.path.isdir(filepath):
            tmp_text_list = readtext_from_dir(filepath)
            text_list.extend(tmp_text_list)
        elif os.path.splitext(filepath)[-1] == '.txt':
            with open(filepath, 'r') as f:
                text_list.append(f.read())
    return text_list

def train(sampledir : str):
    documents = []
    labels = []

    all_sample = os.listdir(sampledir)
    for sample_name in all_sample:
        labels.append(sample_name)

        sample_dirpath = os.path.join(sampledir, sample_name)
        documents.append(readtext_from_dir(sample_dirpath))


    all_texts = []
    all_labels = []

    for i, doc_group in enumerate(documents):
        all_texts.extend(doc_group)
        all_labels.extend([labels[i]] * len(doc_group))

    # TF-IDF
    vectorizer = TfidfVectorizer()
    X = vectorizer.fit_transform(all_texts)

    # K-means Train
    k = len(all_sample)
    kmeans = KMeans(n_clusters=k)
    kmeans.fit(X)

    # assign name for each cluster
    cluster_labels = kmeans.labels_
    cluster_names = {}
    for i in range(k):
        cluster_indices = np.where(cluster_labels == i)[0]
        cluster_labels_for_indices = [all_labels[j] for j in cluster_indices]
        cluster_most_common_label = max(set(cluster_labels_for_indices), key=cluster_labels_for_indices.count)
        cluster_names[i] = cluster_most_common_label

    model_name = os.path.basename(sampledir) + ".pkl"
    model = {"cluster_name" : cluster_names, "kmeans" : kmeans, "tfidf" : vectorizer}
    with open(os.path.join(MODEL_DIR, model_name), 'wb') as f:
        pickle.dump(model, f)
    f.close()

def classify(model_name : str, text : str) -> str:
    with open(os.path.join(MODEL_DIR, model_name), 'rb') as f:
        pkl_data = f.read()
        print(pkl_data)
        model : dict = pickle.loads(pkl_data)
    f.close()

    cluster_names : list = model["cluster_name"]
    kmeans : KMeans = model["kmeans"]
    vectorizer : TfidfVectorizer = model["tfidf"]

    # 对未知文本进行分类
    new_text_vectorized = vectorizer.transform([text])
    predicted_cluster = kmeans.predict(new_text_vectorized)[0]
    return cluster_names[predicted_cluster]
