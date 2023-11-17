---
tags: []
---
# Racing Control Panel Code

Table of Contents
- [[#References]]

---
## Working Notes

[2023-10-28] - I have been struggling with correctly connecting the rotary encoder to the Arduino.  My main concern is that there have been many "phantom" clicks and that has been causing issues when I view (or try to use it) as a joystick button pair because at random intervals I see the inverse direction button pressed when I turn the encoder.

Example Serial Output:
```
Dir: CW
Dir: CW
Dir: CCW
Dir: CW
Dir: CCW
Dir: CW
```

This had me perplexed as even with a hardware debouncer and software debouncing I could not seem to get the inputs to be clean and clear.  In one of my recent experiments I did come across a discovery that my encoder.  I have documented all of this in my [[Rotary Encoders#Usage Experience Notes|Rotary Encode Usage Notes]]



---
## References
- [Video: How to use a Rotary Encoder with an Arduino - CODE EXPLAINED!](https://youtu.be/fgOfSHTYeio?si=N0kpVSW-hEnzY00c)