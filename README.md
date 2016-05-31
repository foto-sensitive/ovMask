# ovMask
This is example shows how you can apply an alpha mask to a 360° video. In post-production a mask is used to hide portions of the layer 
and reveal the layers below. Masking layers is a valuable compositing technique for combining multiple photos into a single image. This 
sketch applies a fragment shader that masks one image based on anothers luminosity to change transparency. It then draws the result onto 
a texture that is unwrapped spherically as a 360° image.

The sketch is part of a series of 360° utility examples I made in openFrameworks, they are distinguished by the 'ov' (omni-vid) prefix.

## Usage
Drag & drop media onto the app window to use as a mask between layers, 

drag mouse across the screen to look around.


## Dependencies:
•ofxGui

### Examples
openFrameworks 0.9.0

should work on any platform, but project files currently exist only for Windows / VS2015
