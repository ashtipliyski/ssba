# Transport, Network and Link Layers

## Exercise: Implement basic traceroute

General notes

- will use a combination of hard TTL limits and ICMP packets to probe the network topology between client and a given host
- client makes a series of UDP requests with gradually increasing TTL limits until the host is found 
- the procedure starts with TTL = 1 which means that the UDP request will fail after the first hop and the client will receive an ICMP packet with the TTL error
- the client can use the time difference between the UDP request and ICMP packet to time the latency to that hop
- each hop is repeated three times (in order to better account for variation in the latency)
- the TTL is increased in steps of one in order to reach the next hop
- when the host is reached, it will reply with an ICMP packet with a destination port error (the original UDP request is configured to use inaccessible port, so that the client app can recognise when the destination is reached)
