import time
import requests

url = "http://192.168.100.9:8081/"

# GET request
def get_request():
    try:
        response = requests.get(url)
        if response.status_code == 200:
            print("GET request successful")
            print("Response:", response.text)
        else:
            print(f"GET request failed with status code {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"GET request failed with exception: {e}")

# POST request
def post_request(data):
    try:
        response = requests.post(url, json=data)
        if response.status_code == 200:
            print("POST request successful")
            print("Response:", response.text)
        else:
            print(f"POST request failed with status code {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"POST request failed with exception: {e}")

if __name__ == "__main__":
    get_request()

    # Example data to send in POST request
    data = {

    "id": 237,
    "name": "User237",
    "weight": "72.5",
    "height": "180.2",
    "age": "28",
    "gender": "F",
    "bloodPressure": "118",
    "activityLevel": "3.6"

    }
    for i in range (0, 10000):
        #post_request(data)
        get_request()
        #time.sleep(1)
