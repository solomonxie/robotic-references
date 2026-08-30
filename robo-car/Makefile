.PHONY: detect deploy dashboard check-deps

check-deps:
	@command -v arduino-cli >/dev/null || { echo "Installing arduino-cli..."; brew install arduino-cli; }

detect: check-deps
	@./detect.sh

deploy: check-deps
	@./deploy.sh "$(F)" "$(SPEED)"

dashboard:
	@[ -d venv ] || python3 -m venv venv
	@venv/bin/pip install -q -r requirements.txt
	@venv/bin/python app.py
