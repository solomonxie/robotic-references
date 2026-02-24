.PHONY: detect deploy check-deps

check-deps:
	@command -v arduino-cli >/dev/null || { echo "Installing arduino-cli..."; brew install arduino-cli; }

detect: check-deps
	@./detect.sh

deploy: check-deps
	@./deploy.sh $(filter-out $@,$(MAKECMDGOALS)) "$(SPEED)"

%:
	@:
