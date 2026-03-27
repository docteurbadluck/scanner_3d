import requests
import sys

urls = [
    "http://localhost:8080",
    "http://localhost:8080/metrics.html",
    "http://localhost:8080/productivity.html",
    "http://localhost:8080/index.html",
]


def test_page(url):
    try:
        response = requests.get(url, timeout=5)
        
        if response.status_code // 100 == 2:
            return True
        else:
            print(f"Failed: {url} returned an error (Status: {response.status_code})")
            return False
    
    except requests.exceptions.RequestException as e:
        print(f"Error: {url} is not reachable. Error details: {e}")
        return False

def ping_pages():
    all_ok = True
    
    for url in urls:
        if not test_page(url):
            all_ok = False 
    
    if all_ok:
        print(f"ping test passed")
        return 0
    else:
        return 1

# Run the script and exit with the appropriate status code
if __name__ == "__main__":
    exit_code = ping_pages()
    sys.exit(exit_code)