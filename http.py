import aiohttp
import asyncio
import argparse
import time

# Funcția care efectuează cererea HTTP
async def fetch(session, url):
    try:
        async with session.get(url) as response:
            return await response.text()
    except Exception as e:
        print(f"Error: {e}")

# Funcția principală care gestionează conexiunile
async def main(url, num_connections):
    # Crearea unui client HTTP asyncron
    async with aiohttp.ClientSession() as session:
        tasks = []
        for _ in range(num_connections):
            # Adăugăm cererile la lista de sarcini
            task = asyncio.ensure_future(fetch(session, url))
            tasks.append(task)

        # Așteptăm finalizarea tuturor sarcinilor
        responses = await asyncio.gather(*tasks)
        print(f"Completed {len(responses)} requests")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Send multiple HTTP requests to a target URL.')
    parser.add_argument('url', type=str, help='The target URL to send requests to')
    parser.add_argument('num_connections', type=int, help='The number of simultaneous connections')

    args = parser.parse_args()
    target_url = args.url
    num_connections = args.num_connections

    start_time = time.time()
    asyncio.run(main(target_url, num_connections))
    end_time = time.time()

    print(f"Total time: {end_time - start_time} seconds")
