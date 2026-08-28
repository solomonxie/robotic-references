.PHONY: flash provision check-deps

check-deps:
	@command -v ansible-playbook >/dev/null || { echo "Installing ansible..."; brew install ansible; }

flash:
	@./flash.sh "$(IMG)" "$(DISK)"

provision: check-deps
	@ansible-playbook -i ../ansible/inventory.ini ../ansible/site.yml --limit nanopi
