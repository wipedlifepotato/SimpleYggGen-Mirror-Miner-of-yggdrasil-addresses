/*
This file generates crypto keys for [ansible-yggdrasil](https://github.com/jcgruenhage/ansible-yggdrasil/)
*/
package main

import (
	"encoding/hex"
	"strings"
	"fmt"
	"net"
	"os"

	//"github.com/cheggaaa/pb/v3"
	"github.com/yggdrasil-network/yggdrasil-go/src/address"
	"github.com/yggdrasil-network/yggdrasil-go/src/crypto"
)


type keySet struct {
	priv []byte
	pub  []byte
	id   []byte
	ip   string
}

func main() {
	args := os.Args[1:]

	if len(args) < 1	{
		fmt.Println("IPv6-addr ");
		return
	}
	
	
	for ; ; {
		keys:=newBoxKey()
		if strings.Contains(keys.ip, args[0]) {
			fmt.Println("Found: "+	keys.ip)
			fmt.Println("Your privkey: "+	hex.EncodeToString(keys.priv) )
			fmt.Println("Your pubkey: "+	hex.EncodeToString(keys.pub) )
			fmt.Println("Your ID: "+	hex.EncodeToString(keys.id) )
			break;
		}
		fmt.Println("Priv: "+hex.EncodeToString(keys.priv) )
		fmt.Println("Pub: "+hex.EncodeToString(keys.pub) )
		fmt.Println("ID: "+hex.EncodeToString(keys.id) )
		fmt.Println("IP: "+keys.ip)
		fmt.Println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	}
}

func newBoxKey() keySet {
	pub, priv := crypto.NewBoxKeys()
	id := crypto.GetNodeID(pub)
	ip := net.IP(address.AddrForNodeID(id)[:]).String()
	return keySet{priv[:], pub[:], id[:], ip}
}

func newSigKey() keySet {
	pub, priv := crypto.NewSigKeys()
	id := crypto.GetTreeID(pub)
	return keySet{priv[:], pub[:], id[:], ""}
}

func isBetter(oldID, newID []byte) bool {
	for idx := range oldID {
		if newID[idx] > oldID[idx] {
			return true
		}
		if newID[idx] < oldID[idx] {
			return false
		}
	}
	return false
}

func sortKeySetArray(sets []keySet) []keySet {
	for i := 0; i < len(sets); i++ {
		sets = bubbleUpTo(sets, i)
	}
	return sets
}

func bubbleUpTo(sets []keySet, num int) []keySet {
	for i := 0; i < len(sets)-num-1; i++ {
		if isBetter(sets[i+1].id, sets[i].id) {
			var tmp = sets[i]
			sets[i] = sets[i+1]
			sets[i+1] = tmp
		} else {
			break
		}
	}
	return sets
}
