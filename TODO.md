END GOAL
    - fully distributed ray tracer
        - worker / leader separation
        - figure out how to send data from worker to leader - unix / TCP sockets

        - step 1 - sharding (to send the data isefl)
            - first step
                - send the data raw - serialize and send over the wire
                - receive the data, deserialize into original objects


TODO
    - Move object serialization from World to materials/register.{cpp,hpp} (should probably be renamed to hittables.{cpp,hpp})
    - Clean up serialization code so much, right now it just does += and manually finds all the field sizes its rlly rlly bad
        - Maybe have some Hittable::size_in_bytes method? and use that.
