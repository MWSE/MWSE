echo "Activating virtual environment..."

python -m venv .venv

echo "Installing dependencies..."

.venv\Scripts\python.exe -m pip install -r requirements.txt --require-virtualenv --disable-pip-version-check

echo "Done!"