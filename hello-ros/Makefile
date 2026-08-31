IMAGE := hello-ros
CONTAINER := hello-ros-dev

.PHONY: build shell exec run stop

build:
	docker build -t $(IMAGE) .

# Start a long-lived container to run one or more nodes in (needed for
# multi-node steps like 2+3 or 5+6 -- each node gets its own `make exec`
# into this same container so they can see each other on localhost).
shell: build
	docker run --rm -it --name $(CONTAINER) $(IMAGE) bash

# Run one more node inside the container started by `make shell`.
exec:
	docker exec -it $(CONTAINER) bash -lc \
		"source install/setup.bash && ./install/hello_ros/lib/hello_ros/$(STEP) $(ARGS)"

# Single-node steps: build, run, and clean up in one shot.
run: build
	docker run --rm -it $(IMAGE) bash -lc \
		"source install/setup.bash && ./install/hello_ros/lib/hello_ros/$(STEP) $(ARGS)"

stop:
	docker rm -f $(CONTAINER) 2>/dev/null || true
