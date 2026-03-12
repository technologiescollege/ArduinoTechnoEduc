# About custom expansion type

Users of this library are free to implement its own expansion and use this library
to support new expansion hardware.

To write a library for your new hardware compliant with this Opta BluePrint
library please follow the guide AddCustomExpansion.md

This document is intended to document and explains how "custom type" are created.


## The expansion type

Each Expansion is identified by a type: this is essentially a number identifing
the type of the hardware attached (so that the Opta controller can recognize 
different kind of hardware).

Official Arduino expansions have a predefined type number: for example Arduino
Analog expansion has ALWAYS type equal to EXPANSION_OPTA_ANALOG (which is 4).

Custom expansion do not have a fixed type number, after all how the Opta BluePrint
library can know about expansion that does not exist yet?

## Register expansion

When you use a custom expansion you need (among other things) REGISTER your espansion
so that Opta BluePrint library can know about that.

To do so you call:
`int registerCustomExpansion(std::string pr, makeExpansion_f f, startUp_f su);`
the parameters are esplained in AddCustomExpansion.md (I won't repeat here).

The important point here is that this function always return your the "custom type"
assigned to you custom expansion.

From now your custom expansion has this type associated.

Pay attention: this type can be different depending on your application (in particular
will depend from the number of custom expansion registered and the order they are
registered), you must not rely on a particular value in your code because it is
not guarantee that you will get always the same _number_.

IMPORTANT NOTE: the best approach to register an expansion is to call 
`registerCustomExpansion` _before_ the OptaController.begin() call.
All works also if expansion are registered after the OptaController.begin() however
this approach is slower because it requires the Controller to look for an
espansion with no custom type associated.

## How to retrieve the type of a register expansion

To know what type has been assigned to your custom expansion use:
`int getExpansionType(std::string pr);`
this will retrieve the number assigned via the register function.
In particular you can use something like that:
`OptaController.getExpansionType(<your expansion type>::getProduct());`
since you have to define a "name" for you expansion type via the getProduct static
function.

## different overloads

Please note that `int getExpansionType(std::string pr);` and `uint8_t getExpansionType(uint8_t i);`
function have a complete different meaning: the first one retrieve the "enumerative" number
associated with a particular custom expansion, while the second one will return the type of 
the expansion present at the index i in the expansion chain (this is the same only if the expansion
is really of "that" type).

An expression like this `getExpansionType("MY CUSTOM EXPANSION") == getExpansionType(2)`
will be true only if you registered an expansion which has a product name equal to "MY CUSTOM EXPANSION" 
(btw choose a better name!) and in the position 2 there is actually and physically your custom expansion.

