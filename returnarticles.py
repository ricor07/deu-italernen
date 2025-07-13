#!/usr/bin/env python3
import sys
import json
import logging
import time
import random
from bs4 import BeautifulSoup
import requests
from urllib.parse import urljoin

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(message)s', datefmt='%H:%M:%S', stream=sys.stderr)
logger = logging.getLogger(__name__)

def print_status(message):
    print(f"[{time.strftime('%H:%M:%S')}] {message}", file=sys.stderr, flush=True)

def is_boilerplate_page(text):
    """Detects if the page is likely to be a boilerplate or legal page (German)."""
    common_phrases = [
        "impressum", "datenschutz", "agb", "kontakt", "über uns", "hilfe",
        "rechtliche hinweise", "nutzungsbedingungen", "datenschutzerklärung",
        "cookie-richtlinie", "werbung", "newsletter", "faq", "karriere",
        "unternehmen", "anzeige", "sitemap", "abonnieren", "service",
        "bedingungen", "rechtliches", "kontaktformular"
    ]
    text_lower = text.lower()
    return any(phrase in text_lower for phrase in common_phrases)

def fetch_article_text(article_url):
    try:
        print_status(f"Checking URL: {article_url}")
        headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36'}
        response = requests.get(article_url, headers=headers, timeout=30)
        print_status(f"Got response from {article_url} (status: {response.status_code})")
        soup = BeautifulSoup(response.text, 'html.parser')

        # Remove unwanted elements
        for elem in soup(['script', 'style', 'nav', 'header', 'footer', 'aside', 'meta']):
            elem.decompose()

        title = soup.find('h1') or soup.find('title')
        title_text = title.get_text(strip=True) if title else ""

        # Find article content
        article = soup.find('article') or soup.find(class_='article-content') or soup.body
        if not article:
            return None

        paragraphs = article.find_all('p')
        content = '\n\n'.join(p.get_text(strip=True) for p in paragraphs if p.get_text(strip=True))

        # Check minimum length and boilerplate
        if not content or not title_text or len(content) < 750:
            return None
        if is_boilerplate_page(content[:500]):
            return None

        return {'title': title_text, 'text': content, 'url': article_url}
    except Exception as e:
        print_status(f"Error fetching article: {e}")
        return None

def process_paper(paper, max_needed):
    try:
        print_status(f"\n=== Processing {paper['name']} ===")
        response = requests.get(paper["url"], timeout=30)
        soup = BeautifulSoup(response.text, 'html.parser')
        links = list(set(a['href'] for a in soup.find_all('a', href=True)))
        random.shuffle(links)

        results = []
        for link in links:
            if len(results) >= max_needed:
                break
            full_url = urljoin(paper["url"], link)
            article_data = fetch_article_text(full_url)
            if article_data:
                results.append(article_data)
        return results
    except Exception as e:
        print_status(f"Error processing paper: {e}")
        return []

def main():
    try:
        input_data = sys.stdin.read()
        data = json.loads(input_data)
        papers = data["papers"]
        num_articles = data["num_articles"]

        all_results = []
        for paper in papers:
            remaining = num_articles - len(all_results)
            if remaining <= 0:
                break
            results = process_paper(paper, remaining)
            all_results.extend(results)

        print(json.dumps(all_results, ensure_ascii=False), flush=True)
    except Exception as e:
        print_status(f"Fatal error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
