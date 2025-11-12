END GOAL
    - fully distributed ray tracer
        - worker / leader separation
        - figure out how to send data from worker to leader - unix / TCP sockets

        - step 1 - sharding (to send the data isefl)
            - first step
                - send the data raw - serialize and send over the wire
                - receive the data, deserialize into original objects


SERIALIZER SETUP
    - Serializer(Object)
        - Get HittableList from Hittable, serialize each of the ones within. Pass them a vector for MaterialList as well so they can fill that out (which std shared ptrs they have).
            - For each Hittable, call bytes()
              - (what does bytes do) It returns a 1-byte prefix, indicating the type of thing it is (sphere, etc). it appends that + data. 

    - Material list
        - After this is done, we should have
