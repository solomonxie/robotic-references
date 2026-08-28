.PHONY: detect deploy

detect:
	@./detect.sh

deploy:
	@./deploy.sh $(filter-out $@,$(MAKECMDGOALS)) "$(SPEED)"

%:
	@:
