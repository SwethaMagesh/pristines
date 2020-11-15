set ns [new Simulator]
set nf [open out.nam w]
$ns namtrace-all $nf
set intv 0.05
set delay [expr $intv+0.02]
set itr 3
 
proc finish {} {
    global ns nf
    $ns flush-trace
    close $nf
    exec nam out.nam &
    exit 0
}
 
set client [$ns node]
set dnsResolver [$ns node]
set rootServer [$ns node]
set tldServer [$ns node]
set authServer [$ns node]
 
$client label "CLIENT"
$dnsResolver label "DNS RESOLVER"
$rootServer label "ROOT NAME SERVER"
$tldServer label "TLD NAME SERVER"
$authServer label "AUTHORITATIVE NS"
 
$client color green
$dnsResolver color blue
$rootServer color red
$tldServer color brown
$authServer color purple
 
$ns duplex-link $client $dnsResolver 2Mb 20ms DropTail
$ns duplex-link $dnsResolver $rootServer 2Mb 20ms DropTail
$ns duplex-link $dnsResolver $tldServer 2Mb 20ms DropTail
$ns duplex-link $dnsResolver $authServer 2Mb 20ms DropTail
 
set clientUdp [new Agent/UDP]
set clientNull [new Agent/Null]
set clientData [new Application/Traffic/CBR]
$clientData attach-agent $clientUdp 
$clientData set interval_ $intv
$ns attach-agent $client $clientUdp
$ns attach-agent $client $clientNull
 
set dnsResolverUdp [new Agent/UDP]
set dnsResolverNull [new Agent/Null]
set dnsResolverData [new Application/Traffic/CBR]
$dnsResolverData attach-agent $dnsResolverUdp 
$dnsResolverData set interval_ $intv
$ns attach-agent $dnsResolver $dnsResolverUdp
$ns attach-agent $dnsResolver $dnsResolverNull
 
set rootServerUdp [new Agent/UDP]
set rootServerNull [new Agent/Null]
set rootServerData [new Application/Traffic/CBR]
$rootServerData attach-agent $rootServerUdp 
$rootServerData set interval_ $intv
$ns attach-agent $rootServer $rootServerUdp
$ns attach-agent $rootServer $rootServerNull
 
set tldServerUdp [new Agent/UDP]
set tldServerNull [new Agent/Null]
set tldServerData [new Application/Traffic/CBR]
$tldServerData attach-agent $tldServerUdp 
$tldServerData set interval_ $intv
$ns attach-agent $tldServer $tldServerUdp
$ns attach-agent $tldServer $tldServerNull
 
set authServerUdp [new Agent/UDP]
set authServerNull [new Agent/Null]
set authServerData [new Application/Traffic/CBR]
$authServerData attach-agent $authServerUdp 
$authServerData set interval_ $intv
$ns attach-agent $authServer $authServerUdp
$ns attach-agent $authServer $authServerNull
 
proc query_response { src dst cbr0 } {
    global ns
    global intv
    $ns connect $src $dst
    set now [$ns now]
    $ns at $now "$cbr0 start"
    $ns at [expr $now+$intv] "$cbr0 stop"
}
 
puts "No of DNS Queries = $itr"
set now [$ns now]
for {set i 0} {$i < $itr} { incr i} {
    set now [expr 9*$i*$delay]
    puts "\nDNS QUERY [expr $i+1] at [expr $now+1*$delay]"
    $ns at [expr $now+1*$delay] "query_response $clientUdp $dnsResolverNull $clientData"
    $ns at [expr $now+2*$delay] "query_response $dnsResolverUdp $rootServerNull $dnsResolverData"
    $ns at [expr $now+3*$delay] "query_response $rootServerUdp $dnsResolverNull $rootServerData"
    $ns at [expr $now+4*$delay] "query_response $dnsResolverUdp $tldServerNull $dnsResolverData"
    $ns at [expr $now+5*$delay] "query_response $tldServerUdp $dnsResolverNull $tldServerData"
    $ns at [expr $now+6*$delay] "query_response $dnsResolverUdp $authServerNull $dnsResolverData"
    $ns at [expr $now+7*$delay] "query_response $authServerUdp $dnsResolverNull $authServerData"
    $ns at [expr $now+8*$delay] "query_response $dnsResolverUdp $clientNull $dnsResolverData"
    puts "DNS RESPONSE [expr $i+1] at [expr $now+9*$delay]"
}
 
$ns at [expr 10*$itr*$delay] "finish"
$ns run