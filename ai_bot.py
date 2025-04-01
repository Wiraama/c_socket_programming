import google.generativeai as genai
import sys
import os
my_secret = os.environ['secret_key']

genai.configure(api_key=my_secret)
def gen_response(prompt):
  model = genai.GenerativeModel("gemini-1.5-pro")
  response = model.generate_content(prompt)
  print(response.text)

if __name__ == "__main__":
  n = len(sys.argv) - 1
  gen_response(sys.argv[n])