import requests
import threading

# Adresa serverului tău (înlocuiește cu adresa reală)
URL = 'http://extreamcs.com'

# Numărul de cereri de trimis
REQUEST_COUNT = 100

def send_request():
    try:
        response = requests.get(URL)
        print(f'Status code: {response.status_code}')
    except requests.RequestException as e:
        print(f'Error: {e}')

def main():
    threads = []
    
    for _ in range(REQUEST_COUNT):
        thread = threading.Thread(target=send_request)
        thread.start()
        threads.append(thread)
    
    for thread in threads:
        thread.join()

if __name__ == "__main__":
    main()
