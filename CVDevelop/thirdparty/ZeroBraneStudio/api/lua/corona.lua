-- Original author Sergey Lerg; updates by Paul Kulchenko
-- Converted from CoronaSDK-APIDOC-2020.3606;
-- the conversion script is at the bottom of this file

-- To process:
-- 1. extract `CoronaSDK-APIDOC-year-build.zip`
-- 2. copy `library`, `type`, and `event` folders to `ZBS/api/lua` folder
-- 3. run "../../bin/lua corona.lua >newapi" from ZBS/api/lua folder
-- 4. copy the content of "newapi" file to replace "api" table in corona.lua
-- 4. launch the IDE and switch to `corona` interpreter to confirm it's loading without issues

local api = {
 _BitmapPaint = {
  childs = {
   rotation = {
    description = "Defines the rotation of the BitmapPaint image.",
    type = "value"
   },
   scaleX = {
    description = "Defines the __x__ scale factor of the BitmapPaint image, for example, `1` equals 100%, `0.5` equals 50%, and `2` equals 200%.",
    type = "value"
   },
   scaleY = {
    description = "Defines the __y__ scale factor of the BitmapPaint image, for example, `1` equals 100%, `0.5` equals 50%, and `2` equals 200%.",
    type = "value"
   },
   x = {
    description = "This value is used to offset a repeating bitmap fill by a ratio of its width.",
    type = "value"
   },
   y = {
    description = "This value is used to offset a repeating bitmap fill by a ratio of its height.",
    type = "value"
   }
  },
  description = "A bitmap paint contains a single texture.",
  inherits = "_Paint",
  type = "class"
 },
 _Body = {
  childs = {
   angularDamping = {
    description = "The numerical value for how much the body's rotation should be damped. The default is zero (`0`). There is no minimum or maximum value - just adjust to get the desired results.",
    type = "value"
   },
   angularVelocity = {
    description = "The numerical value of the current angular (rotational) velocity, in degrees per second. There is no minimum or maximum value - just adjust to get the desired results.",
    type = "value"
   },
   applyAngularImpulse = {
    args = "( appliedForce )",
    description = "Similar to object:applyTorque() except that an angular impulse is a single momentary jolt.",
    returns = "()",
    type = "method"
   },
   applyForce = {
    args = "( xForce, yForce, bodyX, bodyY )",
    description = "A function that accepts __x__ and __y__ components of a linear force, applied at a given point with __x__ and __y__ world coordinates. If the target point is the body's center of mass, it will tend to push the body in a straight line; if the target is offset from the body's center of mass, the body will spin about its center of mass.",
    returns = "()",
    type = "method"
   },
   applyLinearImpulse = {
    args = "( xForce, yForce, bodyX, bodyY )",
    description = "Similar to object:applyForce() except that an impulse is a single momentary jolt.",
    returns = "()",
    type = "method"
   },
   applyTorque = {
    args = "( appliedForce )",
    description = "A function that accepts a numerical value for applied rotational force. The body will rotate about its center of mass.",
    returns = "()",
    type = "method"
   },
   bodyType = {
    description = "A string value for the type of physical body being simulated. Possible values include:",
    type = "value"
   },
   getLinearVelocity = {
    args = "()",
    description = "A function that returns the __x__ and __y__ components for the body's linear velocity, in pixels per second.",
    returns = "(Numbers)",
    type = "method"
   },
   getMassLocalCenter = {
    args = "()",
    description = "A function that returns the __x__ and __y__ components for the body's local center of mass.",
    returns = "(Numbers)",
    type = "method"
   },
   getMassWorldCenter = {
    args = "()",
    description = "A function that returns the __x__ and __y__ components for the body's center of mass in content coordinates.",
    returns = "(Numbers)",
    type = "method"
   },
   gravityScale = {
    description = "Use the `gravityScale` property to adjust the gravity on a single body. For example, setting it to `0` makes the body float (no gravity). The default value is `1.0` which means the body will behave under the normal simulated gravity.",
    type = "value"
   },
   isAwake = {
    description = "A boolean for the body's current \"awake\" state. By default, all bodies automatically \"sleep\" when nothing interacts with them for a couple of seconds. At this point, they stop being simulated until something like a collision wakes them up. The `isAwake` property can either fetch a body's current state or forcibly wake it up.",
    type = "value"
   },
   isBodyActive = {
    description = "Used to set or get the body's current active state. Inactive bodies are not destroyed but they are removed from the physics simulation and cease to interact with other bodies.",
    type = "value"
   },
   isBullet = {
    description = "A boolean for whether the body should be treated as a \"bullet.\" Bullets are subject to continuous collision detection rather than periodic collision detection at world time steps. This is more computationally expensive but it can prevent <nobr>fast-moving</nobr> objects from passing through solid barriers. The default is `false`.",
    type = "value"
   },
   isFixedRotation = {
    description = "A boolean for whether the rotation of the body should be locked, even if the body is under load or subjected to <nobr>off-center</nobr> forces. The default is `false`.",
    type = "value"
   },
   isSensor = {
    description = "A sensor is a fixture that detects collisions but does not produce a physical response. Sensors do not generate contact points.",
    type = "value"
   },
   isSleepingAllowed = {
    description = "A boolean for whether a body is allowed to \"sleep.\" The default is `true`.",
    type = "value"
   },
   linearDamping = {
    description = "The numerical value for how much the body's linear motion is damped. The default is zero (`0`).",
    type = "value"
   },
   mass = {
    description = "A read-only value representing the body's mass.",
    type = "value"
   },
   resetMassData = {
    args = "()",
    description = "If the default mass data for the body has been overridden, this function resets it to the mass calculated from the shapes.",
    returns = "()",
    type = "method"
   },
   setLinearVelocity = {
    args = "( xVelocity, yVelocity )",
    description = "This function accepts __x__ and __y__ components for the body's linear velocity, in pixels per second.",
    returns = "()",
    type = "method"
   }
  },
  description = "Body objects are what interact with the physics world in Corona, are attached to display objects.",
  type = "class"
 },
 _ButtonWidget = {
  childs = {
   getLabel = {
    args = "()",
    description = "Returns the ButtonWidget label text as a string.",
    returns = "(String)",
    type = "method",
    valuetype = "string"
   },
   setEnabled = {
    args = "( isEnabled )",
    description = "Sets the ButtonWidget as either enabled or disabled.",
    returns = "()",
    type = "method"
   },
   setLabel = {
    args = "( label )",
    description = "Sets/updates the ButtonWidget label text.",
    returns = "()",
    type = "method"
   }
  },
  description = "ButtonWidget objects are created using widget.newButton().",
  inherits = "_GroupObject",
  type = "class"
 },
 _CirclePath = {
  childs = {},
  description = "The circle path specifies the geometry of the corresponding circle object.",
  inherits = "_Path",
  type = "class"
 },
 _CompositePaint = {
  childs = {},
  description = "Composite paints contain multiple images, thus enabling multi-texturing.",
  inherits = "_Paint",
  type = "class"
 },
 _CoronaLibrary = {
  childs = {
   getCurrentProvider = {
    args = "()",
    description = "Returns the currently set provider for the library.",
    returns = "(CoronaProvider)",
    type = "method",
    valuetype = "_CoronaProvider"
   },
   name = {
    description = "The name of the library.",
    type = "value"
   },
   publisherId = {
    description = "The name of the publisher.",
    type = "value"
   },
   revision = {
    description = "The revision number of the library.",
    type = "value"
   },
   setCurrentProvider = {
    args = "( provider )",
    description = "Sets the current provider for the library.",
    returns = "()",
    type = "method"
   },
   version = {
    description = "The version number of the library.",
    type = "value"
   }
  },
  description = "This is the standard type for Corona libraries in Lua.",
  type = "class"
 },
 _CoronaPrototype = {
  childs = {
   initialize = {
    args = "()",
    description = "Subclasses can override this method to provide custom initialization of instances.",
    returns = "()",
    type = "method"
   },
   instanceOf = {
    args = "( class )",
    description = "Determines if `object` is an instance of `class`.",
    returns = "(Boolean)",
    type = "method"
   },
   isClass = {
    args = "()",
    description = "Tells you whether `object` is really a class.",
    returns = "(Boolean)",
    type = "method"
   },
   isRoot = {
    args = "()",
    description = "Tells you whether `object` is the root class, i.e. CoronaPrototype.",
    returns = "(Boolean)",
    type = "method"
   },
   new = {
    args = "()",
    description = "Constructor for creating new object instances. The `object` is assumed to be a class object.",
    returns = "(CoronaPrototype)",
    type = "method",
    valuetype = "_CoronaPrototype"
   },
   newClass = {
    args = "( name )",
    description = "Constructor for creating new class objects. The `object` is assumed to be a class object.",
    returns = "(CoronaClass)",
    type = "method"
   },
   setExtension = {
    args = "( indexFunc )",
    description = "Objects based on CoronaPrototype cannot have their `__index` metamethod overridden.",
    returns = "()",
    type = "method"
   }
  },
  type = "class"
 },
 _CoronaProvider = {
  childs = {},
  inherits = "_CoronaPrototype",
  type = "class"
 },
 _DisplayObject = {
  childs = {
   alpha = {
    description = "This property represents the alpha value of a display object. Use it to set or retrieve the object's opacity. A value of `0` is transparent and `1.0` is fully opaque.",
    type = "value"
   },
   anchorX = {
    description = "This property allows you to control the alignment of the object along the __x__ direction.",
    type = "value"
   },
   anchorY = {
    description = "This property allows you to control the alignment of the object along the __y__ direction.",
    type = "value"
   },
   blendMode = {
    description = "Allows you to change the blend mode on a specific object. ",
    type = "value"
   },
   contentBounds = {
    description = "A read-only table with properties `xMin`, `xMax`, `yMin`, `yMax` that represent the boundaries of a display object, in content coordinates.",
    type = "value"
   },
   contentHeight = {
    description = "The read-only height of the object in content coordinates. This is similar to object.height except that its value is affected by __y__ scaling and rotation.",
    type = "value"
   },
   contentToLocal = {
    args = "( xContent, yContent )",
    description = "Maps the given __x__ and __y__ values in content (stage) coordinates to the target object's local coordinates (center point).",
    returns = "(Numbers)",
    type = "method"
   },
   contentWidth = {
    description = "The read-only width of the object in content coordinates. This is similar to object.width except that its value is affected by __x__ scaling and rotation.",
    type = "value"
   },
   defined = {
    description = "Read-only property that provides the file and line number where the object was created.",
    type = "value"
   },
   height = {
    description = "Retrieve or change the height of a display object. For text objects, this property can be used to get (but not set) the height.",
    type = "value"
   },
   isHitTestMasked = {
    description = "Limits touch events to the masked portion of the object. This property can be read or set.",
    type = "value"
   },
   isHitTestable = {
    description = "Allows an object to continue to receive hit events even if it is not visible. If `true`, objects will receive hit events regardless of visibility; if `false`, events are only sent to visible objects. Defaults to `false`.",
    type = "value"
   },
   isVisible = {
    description = "Controls whether the object is visible on the screen. The property is also readable. The default is `true`.",
    type = "value"
   },
   lastChange = {
    description = "Read-only property that provides the file and line number of the last change to any of the object's properties.",
    type = "value"
   },
   localToContent = {
    args = "( x, y )",
    description = "Maps the given __x__ and __y__ coordinates of an object to content (stage) coordinates.",
    returns = "(Numbers)",
    type = "method"
   },
   maskRotation = {
    description = "Retrieve or set the rotation of the display object's corresponding mask object, if one exists.",
    type = "value"
   },
   maskScaleX = {
    description = "Retrieve or set the __x__-scale factor for the display object's corresponding mask object, if any.",
    type = "value"
   },
   maskScaleY = {
    description = "Retrieves or sets the __y__-scale factor for the display object's corresponding mask object, if any.",
    type = "value"
   },
   maskX = {
    description = "Retrieve or set the __x__ position of the mask.",
    type = "value"
   },
   maskY = {
    description = "Retrieve or set the __y__ position of the mask.",
    type = "value"
   },
   parent = {
    description = "A read-only property that returns the object's parent.",
    type = "value"
   },
   properties = {
    description = "Read-only property that provides a view into all of the object's property values.",
    type = "value"
   },
   removeSelf = {
    args = "()",
    description = "Removes the display object and frees its memory, assuming there are no other references to it. This is equivalent to calling group:remove() on the same display object, but it is syntactically simpler. The `object:removeSelf()` syntax is also supported in other cases, such as removing physics joints in the physics engine.",
    returns = "(DisplayObject)",
    type = "method",
    valuetype = "_DisplayObject"
   },
   rotate = {
    args = "( deltaAngle )",
    description = "Effectively adds a value (`deltaAngle`) to the object's current rotation. This rotation is based on degrees in the clockwise direction. The rotation occurs around the object's anchor point.",
    returns = "()",
    type = "method"
   },
   rotation = {
    description = "Sets the rotation of an object in degrees; this rotation is based in the clockwise direction where `0` is directly upward. The rotation occurs around the object's anchor point.",
    type = "value"
   },
   scale = {
    args = "( xScale, yScale )",
    description = "Effectively multiplies the size of a display object by `xScale` and `yScale` respectively. The scaling occurs around the object's anchor point.",
    returns = "()",
    type = "method"
   },
   setMask = {
    args = "( mask )",
    description = "Associates a mask with a display object. To remove an object's mask, use `object:setMask( nil )`. You can modify a display object's mask __x__ and __y__ position (object.maskX).",
    returns = "()",
    type = "method"
   },
   toBack = {
    args = "()",
    description = "Moves the target object to the visual back of its parent group (object.parent).",
    returns = "()",
    type = "method"
   },
   toFront = {
    args = "()",
    description = "Moves the target object to the visual front of its parent group (object.parent).",
    returns = "()",
    type = "method"
   },
   translate = {
    args = "( deltaX, deltaY )",
    description = "Effectively adds values to the object.x properties of an object, thus changing its screen position.",
    returns = "()",
    type = "method"
   },
   width = {
    description = "Retrieve or change the width of a display object. For text objects, this property can be used to get (but not set) the width.",
    type = "value"
   },
   x = {
    description = "Specifies the __x__ position (in local coordinates) of the object relative to its parent point relative to its parent. Changing this value will move the object in the __x__ direction.",
    type = "value"
   },
   xScale = {
    description = "Retrieve or change the scale of the object in the __x__ direction. The scaling occurs around the object's anchor point.",
    type = "value"
   },
   y = {
    description = "Specifies the __y__ position (in local coordinates) of the object relative to its parent point relative to its parent. Changing this value will move the object in the __y__ direction.",
    type = "value"
   },
   yScale = {
    description = "Retrieve or change the scale of the object in the __y__ direction. The scaling occurs around the object's anchor point.",
    type = "value"
   }
  },
  description = "All drawing that occurs on the screen is accomplished by creating display objects. Anything that appears on the screen is an instance of a display object.",
  inherits = "_EventDispatcher",
  type = "class"
 },
 _EmitterObject = {
  childs = {
   pause = {
    args = "()",
    description = "Pauses (freezes) the EmitterObject. No particles will be added, removed, or animated while an emitter is paused.",
    returns = "()",
    type = "method"
   },
   start = {
    args = "()",
    description = "Starts the emission of particles from an EmitterObject.",
    returns = "()",
    type = "method"
   },
   stop = {
    args = "()",
    description = "Stops the emission of particles from an EmitterObject. This function allows the currently active particles to finish their cycle naturally.",
    returns = "()",
    type = "method"
   }
  },
  description = "",
  inherits = "_DisplayObject",
  type = "class"
 },
 _EventDispatcher = {
  childs = {
   addEventListener = {
    args = "( eventName, listener )",
    description = "Adds a listener to the object's list of listeners. When the named event occurs, the listener will be invoked and be supplied with a table representing the event.",
    returns = "(Boolean)",
    type = "method"
   },
   dispatchEvent = {
    args = "( event )",
    description = "Dispatches specified `event` to object. The event parameter must be a table with a `name` property which is a string object, if it has a listener registered to receive name events. We recommend you also include a `target` property in your event to the event so that your listener can know which object received the event.",
    returns = "()",
    type = "method"
   },
   removeEventListener = {
    args = "( eventName, listener )",
    description = "Removes the specified listener from the object's list of listeners so that it no longer is notified of events corresponding to the specified event.",
    returns = "(Boolean)",
    type = "method"
   }
  },
  description = "`EventDispatcher` is any DisplayObject object which can receive events.",
  type = "class"
 },
 _File = {
  childs = {
   close = {
    args = "()",
    description = "Closes the open file (file is returned from io.open()).",
    returns = "()",
    type = "method"
   },
   flush = {
    args = "()",
    description = "Commits the file's output buffer. Saves any written data to the file.",
    returns = "()",
    type = "method"
   },
   lines = {
    args = "()",
    description = "Returns an iterator function that, each time it is called, returns a new line from the file.",
    returns = "(Function)",
    type = "method"
   },
   read = {
    args = "( [fmt1] [, fmt2] [, ...] )",
    description = "Reads a file, according to the given formats which specify what to read. For each format, the function returns a string with the characters read, or `nil` if it cannot read data with the specified format. When called without formats, it uses a default format that reads the entire next line.",
    returns = "(String)",
    type = "method",
    valuetype = "string"
   },
   seek = {
    args = "( [mode] [, offset] )",
    description = "Sets and gets the file position, measured from the beginning of the file, to the position given by offset plus a base.",
    returns = "(Number)",
    type = "method"
   },
   setvbuf = {
    args = "( mode [, size ] )",
    description = "Sets the buffering mode for an output file (or console).",
    returns = "()",
    type = "method"
   },
   write = {
    args = "( arg1 [, arg2] [, ...] )",
    description = "Writes the value of each of its arguments to the file. The arguments must be Strings before calling `File:write()`.",
    returns = "()",
    type = "method"
   }
  },
  description = "File objects in Corona are used to interact with (read and write) external files, and are returned from io.open().",
  type = "class"
 },
 _GameNetwork = {
  childs = {
   match = {
    description = "A match object returned from gameNetwork.request().",
    type = "value"
   },
   outcome = {
    description = "Valid string outcome values for the participant.",
    type = "value"
   },
   participant = {
    description = "A participant object returned from gameNetwork.request().",
    type = "value"
   },
   status = {
    description = "Valid string status values for the participant.",
    type = "value"
   }
  },
  description = "The details for various items returned from gameNetwork callbacks.",
  type = "class"
 },
 _GradientPaint = {
  childs = {},
  description = "A gradient paint creates a linear progression from one color to another.",
  inherits = "_Paint",
  type = "class"
 },
 _GroupObject = {
  childs = {
   anchorChildren = {
    description = "By default, display groups do not respect anchor points. However, you can achieve anchor behavior on a display group by setting this property to `true`.",
    type = "value"
   },
   insert = {
    args = "( [index,] child, [, resetTransform] )",
    description = "Inserts an object into a group.",
    returns = "()",
    type = "method"
   },
   numChildren = {
    description = "Retrieve the number of children in a group. You access the children by integer (whole number) index.",
    type = "value"
   },
   remove = {
    args = "( indexOrChild )",
    description = "Remove an object from a group by either an index number or a reference to the object.",
    returns = "(DisplayObject)",
    type = "method",
    valuetype = "_DisplayObject"
   }
  },
  description = "Group objects are a special type of display object. You can add other display objects as children of a group object. You can also remove them. Even if an object is not visible, it remains in the group object until explicitly removed. Thus, to minimize memory consumption, you should explicitly remove any object that will no longer be used.",
  inherits = "_DisplayObject",
  type = "class"
 },
 _ImageSheet = {
  childs = {},
  description = "ImageSheet objects are created using graphics.newImageSheet().",
  type = "class"
 },
 _ImageSheetPaint = {
  childs = {
   frame = {
    description = "Defines the image sheet.",
    type = "value"
   }
  },
  description = "An image sheet paint is a special kind of BitmapPaint frame.",
  inherits = "_Paint",
  type = "class"
 },
 _InputAxis = {
  childs = {
   accuracy = {
    description = "The +/- accuracy of the data provided by the axis input. This value will always be greater than or equal to `0`. For example, if the accuracy is `0.01`, an axis value of `0.5` is accurate between `0.49` and `0.51`.",
    type = "value"
   },
   descriptor = {
    description = "Provides a human-readable string (assigned by Corona) which can be used to uniquely identify one axis input belonging to one device. This descriptor string is intended to be used by applications that set up key and axis bindings with a particular device, such as the first joystick connected to the system.",
    type = "value"
   },
   maxValue = {
    description = "Indicates the maximum value that an axis input event will provide. This property and the object.minValue property are needed to make sense of the raw data received by an axis because it will indicate how far that axis has been moved relative to its range.",
    type = "value"
   },
   minValue = {
    description = "Indicates the minimum value that an axis input event will provide. This property and the object.maxValue property are needed to make sense of the raw data received by an axis because it will indicate how far that axis has been moved relative to its range.",
    type = "value"
   },
   number = {
    description = "The number assigned to an input device's axis. This number is based on the number of axes found on one input device. For example, if an input device has 4 axes, then they will be assigned numbers `1`, `2`, `3`, and `4` in the order that they were found. You can use this number to uniquely identify an axis belonging to one input device. This axis number can be used as an index with the array returned by its input device's getAxes() function.",
    type = "value"
   },
   type = {
    description = "A string describing the type of axis input that an InputDevice(#names) below for a complete list of values.",
    type = "value"
   }
  },
  description = "Provides information about one analog axis input belonging to an InputDevice, such as an <nobr>__x__-axis</nobr> or <nobr>__y__-axis</nobr> of a joystick, a gamepad's left or right analog trigger, etc. The main usage of this object is to acquire the minimum and maximum values that the axis can provide in order to make sense of its data.",
  type = "class"
 },
 _InputDevice = {
  childs = {
   MFiProfile = {
    description = "A string that specifies which MFi controller profile the device conforms to:",
    type = "value"
   },
   allowsRotation = {
    description = "This value is specific to the Apple TV Remote (object.MFiProfile is `\"microGamepad\"`).",
    type = "value"
   },
   androidDeviceId = {
    description = "The unique integer ID assigned to the input device by Android.",
    type = "value"
   },
   canVibrate = {
    description = "Determines if the input device supports vibration/rumble feedback. This is a features that usually gamepads support.",
    type = "value"
   },
   connectionState = {
    description = "Indicates the input device's current connection state with the system such as \"connected\", \"disconnected\", etc.",
    type = "value"
   },
   descriptor = {
    description = "Provides a unique human readable string assigned by Corona used to uniquely identify the device. This descriptor string is intended to be used by applications that set up key bindings with a particular device, such as the first joystick connected to the system.",
    type = "value"
   },
   displayName = {
    description = "The name that was assigned to the input device by the system or end-user. Can be used to display the name of the device to the end-user, such as on a key/button binding screen.",
    type = "value"
   },
   driver = {
    description = "A string which specifies the <nobr>platform-dependent</nobr> backend of the device (if any):",
    type = "value"
   },
   getAxes = {
    args = "()",
    description = "Fetches information about all axis inputs available on the device. This information can be used to detect the device's capabilities, such as whether or not an analog joystick is available.",
    returns = "(Array)",
    type = "method"
   },
   isConnected = {
    description = "Determines if the input device is currently connected to the system and can provide input events.",
    type = "value"
   },
   permanentId = {
    description = "Provides the input device's unique string ID which will be consistent between reboots. That is, the ID for the input device will not change after rebooting the system. This means that this ID can be saved to file and used to set up key bindings with a very specific input device.",
    type = "value"
   },
   playerNumber = {
    description = "Read-only property which returns the player number assigned to a game controller by the system, such as a Windows' XInput (Xbox) game controller or <nobr>Android 5.0+</nobr> game controller. For Windows XInput game controllers, this number ranges between `1` and `4`, typically indicated by a light on the controller. On Android TV and <nobr>Amazon Fire TV</nobr>, this number ranges between `1` and `4` as well.",
    type = "value"
   },
   productName = {
    description = "This is typically the product name assigned to the device by the manufacturer. Unlike the displayName property, the product name will not change at runtime. You can use this name to profile game controllers so that you can safely assume what buttons or axes are available on the device and automatically bind them to your game's controls.",
    type = "value"
   },
   reportsAbsoluteDpadValues = {
    description = "This value is specific to the Apple TV Remote (object.MFiProfile is `\"microGamepad\"`).",
    type = "value"
   },
   type = {
    description = "A string that specifies what kind of device is connected to the system. Possible values are:",
    type = "value"
   },
   vibrate = {
    args = "()",
    description = "Causes the input device to vibrate/rumble.",
    returns = "()",
    type = "method"
   }
  },
  description = "An object of this type respresent one input device such as a keyboard, mouse, touchscreen, gamepad, etc. This object provides access to the input device's information and its current connection status.",
  type = "class"
 },
 _Joint = {
  childs = {
   dampingRatio = {
    description = "The joint's damping ratio. This value can range from `0` (no damping) to `1` (critical damping). With critical damping, all oscillations should vanish.",
    type = "value"
   },
   frequency = {
    description = "This value specifies the mass-spring damping frequency in Hz. For use with `\"distance\"`, `\"touch\"`, `\"weld\"`, and `\"wheel\"` joints.",
    type = "value"
   },
   getAnchorA = {
    args = "()",
    description = "Returns the __x__ and __y__ coordinates of the joint's anchor point within object `A`, where `A` and `B` are the two joined bodies. Values are in content space.",
    returns = "(Numbers)",
    type = "method"
   },
   getAnchorB = {
    args = "()",
    description = "Returns the __x__ and __y__ coordinates of the joint's anchor point within object `B`, where `A` and `B` are the two joined bodies. Values are in content space.",
    returns = "(Numbers)",
    type = "method"
   },
   getGroundAnchorA = {
    args = "()",
    description = "Returns the __x__ and __y__ coordinates of a `\"pulley\"` joint's first ground anchor in content coordinates.",
    returns = "(Numbers)",
    type = "method"
   },
   getGroundAnchorB = {
    args = "()",
    description = "Returns the __x__ and __y__ coordinates of a `\"pulley\"` joint's second ground anchor in content coordinates.",
    returns = "(Numbers)",
    type = "method"
   },
   getLimits = {
    args = "()",
    description = "This function returns the current negative and positive motion limits for a `\"piston\"` joint.",
    returns = "(Numbers)",
    type = "method"
   },
   getLocalAnchorA = {
    args = "()",
    description = "Returns the __x__ and __y__ coordinates of the joint's anchor point within object `A`, where `A` and `B` are the two joined bodies. Values are relative (local) to object `A`.",
    returns = "(Numbers)",
    type = "method"
   },
   getLocalAnchorB = {
    args = "()",
    description = "Returns the __x__ and __y__ coordinates of the joint's anchor point within object `B`, where `A` and `B` are the two joined bodies. Values are relative (local) to object `B`.",
    returns = "(Numbers)",
    type = "method"
   },
   getLocalAxisA = {
    args = "()",
    description = "This function returns the current coordinates of the anchor point along the defined axis, in content space.",
    returns = "(Numbers)",
    type = "method"
   },
   getReactionForce = {
    args = "()",
    description = "This function returns the reaction force in Newtons at the joint anchor in the second body.",
    returns = "(Numbers)",
    type = "method"
   },
   getRotationLimits = {
    args = "()",
    description = "This function returns the current negative and positive rotation limits of a `\"pivot\"` joint.",
    returns = "(Numbers)",
    type = "method"
   },
   getTarget = {
    args = "()",
    description = "Returns the current target coordinates of a `\"touch\"` joint as specified by object:setTarget().",
    returns = "(Numbers)",
    type = "method"
   },
   isActive = {
    description = "Read-only boolean value which indicates whether a joint is active or inactive.",
    type = "value"
   },
   isCollideConnected = {
    description = "Read-only boolean value which specifies if the joined objects follow the standard collision rules (`true`) or not (`false`).",
    type = "value"
   },
   isLimitEnabled = {
    description = "Set this to `true` to constrain the limits of rotation for a `\"pivot\"` joint or the limits of motion for a `\"piston\"` joint.",
    type = "value"
   },
   isMotorEnabled = {
    description = "Boolean value which indicates whether a `\"pivot\"` or `\"piston\"` joint is <nobr>motor-enabled</nobr> or not. Motor action for a `\"pivot\"` joint is rotational, while motor action for a `\"piston\"` joint is linear along the defined axis.",
    type = "value"
   },
   joint1 = {
    description = "Applies only to `\"gear\"` joints. Read-only reference to the first `\"pivot\"` or `\"piston\" joint associated with the gear joint.",
    type = "value"
   },
   joint2 = {
    description = "Applies only to `\"gear\"` joints. Read-only reference to the second `\"pivot\"` or `\"piston\"` joint associated with the gear joint.",
    type = "value"
   },
   jointAngle = {
    description = "Read-only value indicating the current angle of a `\"pivot\"` joint, in degrees.",
    type = "value"
   },
   jointSpeed = {
    description = "Read-only value indicating the current rotation speed of a `\"pivot\"` joint.",
    type = "value"
   },
   jointTranslation = {
    description = "Read-only value indicating the current translation of a `\"piston\"` or `\"wheel\"` joint, meaning, the distance of movement that has occurred along the axis.",
    type = "value"
   },
   length = {
    description = "Defines the distance between the anchor points for a `\"distance\"` joint, which should not be zero or very short. If you position the bodies before applying a distance joint to them, this length will automatically be set as the distance between the anchor points, so it's usually not necessary to set this parameter.",
    type = "value"
   },
   length1 = {
    description = "Read-only value that, upon instantiation of a `\"pulley\"` joint, indicates the distance in pixels between the first anchor point and its stationary pulley anchor point.",
    type = "value"
   },
   length2 = {
    description = "Read-only value that, upon instantiation of a `\"pulley\"` joint, indicates the distance in pixels between the second anchor point and its stationary pulley anchor point.",
    type = "value"
   },
   limitState = {
    description = "Read-only value that indicates the current limit state of a `\"rope\"` joint: `\"inactive\"`, `\"lower\"`, `\"upper\"`, or `\"equal\"`.",
    type = "value"
   },
   maxForce = {
    description = "Applies only to `\"friction\"` and `\"touch\"` joints.",
    type = "value"
   },
   maxLength = {
    description = "For use with `\"rope\"` joints. Specifies the maximum separation length of the two bodies (rope length), in pixels.",
    type = "value"
   },
   maxMotorForce = {
    description = "The maximum allowed force for the motor of a `\"piston\"` joint.",
    type = "value"
   },
   maxMotorTorque = {
    description = "The maximum allowed torque for the motor of a `\"pivot\"` joint.",
    type = "value"
   },
   maxTorque = {
    description = "Applies only to `\"friction\"` joints. Specifies the maximum rotational friction which may be applied to the joined body. A higher value simulates higher friction.",
    type = "value"
   },
   motorForce = {
    description = "Read-only value indicating the current motor force of a `\"piston\"` joint.",
    type = "value"
   },
   motorSpeed = {
    description = "Numerical value which specifies the rotational motor speed for a `\"pivot\"` joint, or the linear motor speed for a `\"piston\"` joint.",
    type = "value"
   },
   motorTorque = {
    description = "Read-only value indicating the current motor torque for a `\"pivot\"` joint.",
    type = "value"
   },
   ratio = {
    description = "Applies only to `\"pulley\"` and `\"gear\"` joints.",
    type = "value"
   },
   reactionTorque = {
    description = "Read-only property which indicates the reaction torque in N*m at the joint anchor in the second body.",
    type = "value"
   },
   referenceAngle = {
    description = "Read-only value indicating the joint angle between the bodies at time of creation. For use with `\"pivot\"`, `\"piston\"`, and `\"weld\"` joints.",
    type = "value"
   },
   removeSelf = {
    args = "()",
    description = "Destroys an existing joint and detaches the two joined bodies. This should not be called during a collision.",
    returns = "()",
    type = "method"
   },
   setLimits = {
    args = "( negLimit, posLimit )",
    description = "This function accepts two values which define the negative and positive range of motion for a `\"piston\"` joint. The second value should always be greater than or equal to the first value, since they define a range of motion (distance) along the axis.",
    returns = "()",
    type = "method"
   },
   setRotationLimits = {
    args = "( negLimit, posLimit )",
    description = "This function accepts two values, in degrees, defining the negative and positive limits of rotation for a `\"pivot\"` joint. For example, if you want to limit the rotation somewhat tightly in relation to the upward axis (`0`), these values may be `-10` and `10` respectively. Note that these values remain in relation to the rotation of body `A`, meaning that if body `A` rotates after the joint is declared, these rotation limits will remain in rotational synchronization with that body.",
    returns = "()",
    type = "method"
   },
   setTarget = {
    args = "( targetX, targetY )",
    description = "Sets the current target (follow) point of a `\"touch\"` joint in content space coordinates. This can be any specific content point, the location of the user's touch, the coordinates of some other object to follow, successive points along a path, etc.",
    returns = "()",
    type = "method"
   },
   springDampingRatio = {
    description = "Applies only to `\"wheel\"` joints. This value can range from `0` (no damping) to `1` (critical damping). With critical damping, all oscillations should vanish.",
    type = "value"
   },
   springFrequency = {
    description = "Applies only to `\"wheel\"` joints. This value specifies the <nobr>mass-spring</nobr> damping frequency in Hz. A low value will decrease simulated suspension.",
    type = "value"
   }
  },
  description = "Joints can be used to assemble complex physical objects from multiple rigid bodies. For example, joints can be used to join the limbs of a ragdoll figure, attach the wheels of a vehicle to its body, create a moving elevator platform, and more. All joints are created using the physics.newJoint() constructor.",
  type = "class"
 },
 _LineObject = {
  childs = {
   anchorSegments = {
    description = "This boolean property controls whether LineObjects into account. This value is `false` by default.",
    type = "value"
   },
   append = {
    args = "( x, y [, ... ] )",
    description = "Append one or more segments to an existing object created using display.newLine().",
    returns = "()",
    type = "method"
   }
  },
  description = "Line objects are created using display.newLine().",
  inherits = "_DisplayObject",
  type = "class"
 },
 _Map = {
  childs = {
   addMarker = {
    args = "( latitude, longitude )",
    description = "Adds a pin to the map at the specified location. The optional title and subtitle will appear on a small popup when the pin is touched.  If a custom image is specified then the bottom center of the image will be the pinned location.",
    returns = "(Number)",
    type = "method"
   },
   getAddressLocation = {
    args = "()",
    description = "This function has been replaced by object:requestLocation().",
    returns = "()",
    type = "method"
   },
   getUserLocation = {
    args = "()",
    description = "Returns a table containing values for the user's current location, including:",
    returns = "(Table)",
    type = "method"
   },
   isLocationVisible = {
    description = "A read-only Boolean value indicating whether the user's current location is visible within the area currently displayed on the map. This is based on an approximation, so it may be that the value is true when the user's position is slightly offscreen.",
    type = "value"
   },
   isScrollEnabled = {
    description = "A Boolean that determines whether users can scroll the map by hand. Default is `true`. Set to `false` to prevent users from scrolling the map. Note that a map can still be scrolled/panned via the object:setCenter() functions, even if this property is `false`. This is useful if you want map movement to be controlled by the program, not the user.",
    type = "value"
   },
   isZoomEnabled = {
    description = "A boolean that determines whether users may use pinch/zoom gestures to zoom the map. Default is `true`. Set to `false` to prevent users from zooming the map. Note that a map can still be zoomed via the object:setRegion() function, even when this property is set to `false`.",
    type = "value"
   },
   mapType = {
    description = "A string that specifies the type of map display. Possible values are:",
    type = "value"
   },
   nearestAddress = {
    args = "( latitude, longitude, resultHandler )",
    description = "Returns the nearest address based on the given latitude and longitude values, returned as a mapAddress event.",
    returns = "()",
    type = "method"
   },
   removeAllMarkers = {
    args = "()",
    description = "Clears all markers (pins) from the map.",
    returns = "()",
    type = "method"
   },
   removeMarker = {
    args = "( markerID )",
    description = "Clears a specific marker (pin) from the map.",
    returns = "()",
    type = "method"
   },
   requestLocation = {
    args = "( location, resultHandler )",
    description = "This is a replacement for the deprecated object:getAddressLocation().",
    returns = "()",
    type = "method"
   },
   setCenter = {
    args = "( latitude, longitude [, isAnimated] )",
    description = "Moves the displayed map region to a new location, using the new center point but maintaining the zoom level. The final parameter is an optional boolean (default `false`) that determines whether the transition is animated or happens instantly.",
    returns = "()",
    type = "method"
   },
   setRegion = {
    args = "( latitude, longitude, latitudeSpan, longitudeSpan [, isAnimated] )",
    description = "Moves the displayed map region to a new location, with the new center point and horizontal/vertical span distances given in degrees of latitude and longitude. This implicitly sets the zoom level. This function will \"sanity-check\" the span settings and will interpolate a consistent zoom level even if `latitudeSpan` and `longitudeSpan` are specified with radically different values. The final parameter is an optional boolean (default `false`) that determines whether the transition is animated or happens instantly.",
    returns = "()",
    type = "method"
   }
  },
  description = "The map view feature lets you integrate one or more map views into your app. It supports various methods of <nobr>two-way</nobr> communication between the map contents and the surrounding application, including address or landmark lookup, marking the map with pins, and reverse geocoding (converting latitude/longitude into the closest street address).",
  type = "class"
 },
 _Mask = {
  childs = {},
  description = "Mask objects are created with graphics.newMask().",
  type = "class"
 },
 _NativeDisplayObject = {
  childs = {
   getNativeProperty = {
    args = "( property )",
    description = "This function allows you to __get__ properties of the underlying native object created by the native on iOS, you can access the <nobr>Obj-C</nobr> properties of the corresponding `WKWebView` (or `WKWebViewConfiguration` if called before a request is made).",
    returns = "()",
    type = "method"
   },
   setNativeProperty = {
    args = "( property, value )",
    description = "This function allows you to __set__ properties of the underlying native object created by the native on iOS, you can set the Obj-C properties of the corresponding `WKWebView` (or `WKWebViewConfiguration` if called before a request is made).",
    returns = "()",
    type = "method"
   }
  },
  description = "Native display objects are separate from Corona's OpenGL drawing canvas and always appear in front of normal display objects.",
  inherits = "_DisplayObject",
  type = "class"
 },
 _Paint = {
  childs = {
   a = {
    description = "The alpha channel value ranging from `0` to `1`.",
    type = "value"
   },
   b = {
    description = "The blue channel value ranging from `0` to `1`.",
    type = "value"
   },
   blendEquation = {
    description = "The blend equation per the OpenGL-ES 2 specification(http://www.khronos.org/opengles/sdk/docs/man/xhtml/glBlendEquation.xml). ",
    type = "value"
   },
   blendMode = {
    description = "Allows you to change the blend mode on a specific object. ",
    type = "value"
   },
   effect = {
    description = "Apply shader effects to an object, including filters, generators (<nobr>procedurally-generated</nobr> textures), and composites (multitexturing).",
    type = "value"
   },
   g = {
    description = "The green channel value ranging from `0` to `1`.",
    type = "value"
   },
   r = {
    description = "The red channel value ranging from `0` to `1`.",
    type = "value"
   }
  },
  description = "For object fills.",
  type = "class"
 },
 _ParticleSystem = {
  childs = {
   applyForce = {
    args = "( xForce, yForce )",
    description = "A function that accepts __x__ and __y__ components of a linear force in newtons, applied to the center of each particle in a ParticleSystem.",
    returns = "()",
    type = "method"
   },
   applyLinearImpulse = {
    args = "( xForce, yForce )",
    description = "Similar to object:applyForce() except that an impulse is a single momentary jolt in <nobr>newton-seconds.</nobr>",
    returns = "()",
    type = "method"
   },
   createGroup = {
    args = "( params )",
    description = "This function is used to create multiple particles simultaneously by filling a region.",
    returns = "()",
    type = "method"
   },
   createParticle = {
    args = "( params )",
    description = "This function is used to create a single particle in a ParticleSystem.",
    returns = "()",
    type = "method"
   },
   destroyParticles = {
    args = "( params )",
    description = "This function is used to remove all particles within a region. It will return a number indicating how many particles were destroyed.",
    returns = "(Number)",
    type = "method"
   },
   imageRadius = {
    description = "This numerical property controls the radius of the particle image, in content units.",
    type = "value"
   },
   particleCount = {
    description = "This read-only numerical property represents the number of particles alive.",
    type = "value"
   },
   particleDamping = {
    description = "This numerical property controls the damping of particles.",
    type = "value"
   },
   particleDensity = {
    description = "This numerical property controls the density of particles. This value is `1.0` by default.",
    type = "value"
   },
   particleDestructionByAge = {
    description = "This boolean property controls the destruction of particles based on their age. This value is `true` by default.",
    type = "value"
   },
   particleGravityScale = {
    description = "This numerical property controls the scaling of gravity. This value is `1.0` by default.",
    type = "value"
   },
   particleMass = {
    description = "Read-only numerical property representing the mass of a single particle, in kilograms, within a ParticleSystem.",
    type = "value"
   },
   particleMaxCount = {
    description = "This numerical property controls the maximum number of particles. This value is `0` by default, indicating no maximum limit.",
    type = "value"
   },
   particlePaused = {
    description = "This boolean property controls pausing of the simulation of particles. This value is `false` by default, meaning that the simulation should be executed as normal.",
    type = "value"
   },
   particleRadius = {
    description = "This numerical property controls the radius of particles. This value is `1.0` by default.",
    type = "value"
   },
   particleStrictContactCheck = {
    description = "This boolean property controls the strict particle/body contact check. This value is `false` by default.",
    type = "value"
   },
   queryRegion = {
    args = "( upperLeftX, upperLeftY, lowerRightX, lowerRightY, hitProperties )",
    description = "This function is used to find the particles that intersect with an <nobr>axis-aligned</nobr> <nobr>(non-rotated)</nobr> box. This box is defined by an <nobr>upper-left</nobr> coordinate and a <nobr>lower-right</nobr> coordinate.",
    returns = "(Array)",
    type = "method"
   },
   rayCast = {
    args = "( from_x, from_y, to_x, to_y, behavior )",
    description = "This function is used to find the particles that collide with a line. It returns an array of tables describing each hit.",
    returns = "(Array)",
    type = "method"
   }
  },
  description = "Particle system objects are created using physics.newParticleSystem().",
  inherits = "_DisplayObject",
  type = "class"
 },
 _Path = {
  childs = {},
  description = "Paths specify the geometry of shapes. These path objects give you access to manipulate this geometry.",
  type = "class"
 },
 _PhysicsContact = {
  childs = {
   bounce = {
    description = "Overrides the normal bounce/restitution of the PhysicsContact event listener.",
    type = "value"
   },
   friction = {
    description = "Overrides the normal friction of the PhysicsContact event listener.",
    type = "value"
   },
   isEnabled = {
    description = "Enable or disable the collision associated with this contact. This is typically used inside a preCollision event listener and may be used to construct <nobr>one-sided</nobr> platforms that the character can jump vertically through, but only from below.",
    type = "value"
   },
   isTouching = {
    description = "Boolean property (read-only) indicating whether the two objects associated with this PhysicsContact are touching.",
    type = "value"
   }
  },
  description = "Physics contacts are created by Box2D to manage/override collision behavior in special cases.",
  type = "class"
 },
 _PickerWheelWidget = {
  childs = {
   getValues = {
    args = "()",
    description = "Returns a table with the currently selected value/index of each column in the PickerWheelWidget.",
    returns = "(Table)",
    type = "method"
   },
   selectValue = {
    args = "( targetColumn, targetIndex [, snapToIndex] )",
    description = "Selects a specific row within a specific column of the PickerWheelWidget. Optionally allows you to snap directly/instantly to the row instead of via default scrolling motion.",
    returns = "()",
    type = "method"
   }
  },
  description = "PickerWheelWidget objects are created using widget.newPickerWheel().",
  inherits = "_GroupObject",
  type = "class"
 },
 _ProgressViewWidget = {
  childs = {
   getProgress = {
    args = "()",
    description = "Returns the current progress value of a ProgressViewWidget.",
    returns = "(Number)",
    type = "method"
   },
   resizeView = {
    args = "( newWidth )",
    description = "Resizes the width of a ProgressViewWidget after creation.",
    returns = "()",
    type = "method"
   },
   setProgress = {
    args = "( progress )",
    description = "Sets the current progress of a ProgressViewWidget.",
    returns = "()",
    type = "method"
   }
  },
  description = "ProgressViewWidget objects are created using widget.newProgressView().",
  inherits = "_GroupObject",
  type = "class"
 },
 _Recording = {
  childs = {
   getSampleRate = {
    args = "()",
    description = "Gets the current audio recording sample rate.",
    returns = "(Number)",
    type = "method"
   },
   isRecording = {
    args = "()",
    description = "Returns `true` if audio recording is currently in progress; `false` if otherwise.",
    returns = "(Boolean)",
    type = "method"
   },
   setSampleRate = {
    args = "( r )",
    description = "Request an audio recording sample rate.",
    returns = "()",
    type = "method"
   },
   startRecording = {
    args = "()",
    description = "Start recording audio.",
    returns = "()",
    type = "method"
   },
   stopRecording = {
    args = "()",
    description = "Stops recording audio.",
    returns = "()",
    type = "method"
   }
  },
  description = "Recording objects are created using media.newRecording().",
  type = "class"
 },
 _RectPath = {
  childs = {},
  description = "The rectangle path specifies the geometry of the corresponding rectangle object.",
  inherits = "_Path",
  type = "class"
 },
 _RoundedRectPath = {
  childs = {},
  description = "The rounded rectangle path specifies the geometry of the corresponding rounded rectangle object.",
  inherits = "_Path",
  type = "class"
 },
 _Runtime = {
  childs = {
   hasEventSource = {
    args = "( eventSourceName )",
    description = "Determines if the device is capable of providing events for a given event source such as `\"accelerometer\"` or `\"gyroscope\"`.",
    returns = "(Boolean)",
    type = "method"
   },
   hideErrorAlerts = {
    args = "()",
    description = "Disables the runtime error alert that appears if the application hits an error condition. This is shorthand for defining your own unhandledError listener and returning `true`.",
    returns = "()",
    type = "method"
   }
  },
  description = "The `Runtime` class inherits from EventDispatcher. It is a singleton class, meaning there is only one instance, accessible via the global runtime.",
  inherits = "_EventDispatcher",
  type = "class"
 },
 _Scene = {
  childs = {},
  description = "Scene objects are the focal point of the Composer.",
  inherits = "_EventDispatcher",
  type = "class"
 },
 _ScrollViewWidget = {
  childs = {
   getContentPosition = {
    args = "()",
    description = "Returns the __x__ and __y__ coordinates of the ScrollViewWidget content.",
    returns = "(Numbers)",
    type = "method"
   },
   getView = {
    args = "()",
    description = "Returns a reference to the ScrollViewWidget object's view.",
    returns = "(Table)",
    type = "method"
   },
   scrollTo = {
    args = "( position, options )",
    description = "Makes a ScrollViewWidget scroll to a specified position constant.",
    returns = "()",
    type = "method"
   },
   scrollToPosition = {
    args = "( options )",
    description = "Makes a ScrollViewWidget scroll to a specific __x__ or __y__ position.",
    returns = "()",
    type = "method"
   },
   setIsLocked = {
    args = "( isLocked [, axis] )",
    description = "Sets a ScrollViewWidget to either locked (does not scroll) or unlocked (default behavior). Optionally, you can lock or unlock the scroll view on a specific directional axis.",
    returns = "()",
    type = "method"
   },
   setScrollHeight = {
    args = "( newHeight )",
    description = "Updates the scrollable content height of a ScrollViewWidget, meaning the total amount that the user can scroll the view in the vertical direction.",
    returns = "(Number)",
    type = "method"
   },
   setScrollWidth = {
    args = "( newWidth )",
    description = "Updates the scrollable content width of a ScrollViewWidget, meaning the total amount that the user can scroll the view in the horizontal direction.",
    returns = "(Number)",
    type = "method"
   },
   takeFocus = {
    args = "( event )",
    description = "If you have an object with a touch listener inside a scroll view such as a ButtonWidget, you should call this method within the `\"moved\"` phase of that object's touch listener and pass the touch event's `event` table as the sole parameter of this method. This will give focus back to the scroll view, enabling it to continue to scroll.",
    returns = "(Number)",
    type = "method"
   }
  },
  description = "ScrollViewWidget objects are created using widget.newScrollView().",
  inherits = "_GroupObject",
  type = "class"
 },
 _SegmentedControlWidget = {
  childs = {
   segmentLabel = {
    description = "A read-only value indicating the label of the currently selected segment on a SegmentedControlWidget.",
    type = "value"
   },
   segmentNumber = {
    description = "A read-only value indicating the segment number of the currently selected segment on a SegmentedControlWidget.",
    type = "value"
   },
   setActiveSegment = {
    args = "( segmentNumber )",
    description = "Sets the active segment for a SegmentedControlWidget.",
    returns = "()",
    type = "method"
   }
  },
  description = "SegmentedControlWidget objects are created using widget.newSegmentedControl().",
  inherits = "_GroupObject",
  type = "class"
 },
 _ShapeObject = {
  childs = {
   fill = {
    description = "For object fills, Corona uses the concept of paint to a fill, you control how the interior area of the shape is rendered.",
    type = "value"
   },
   path = {
    description = "Paths are a property of shapes that let you control the geometry of the shape.",
    type = "value"
   },
   setFillColor = {
    args = "( gray )",
    description = "Sets the fill color of vector and text objects. Also applies a tint to image objects.",
    returns = "()",
    type = "method"
   },
   setStrokeColor = {
    args = "( gray )",
    description = "Sets the stroke (border) color of vector objects.",
    returns = "()",
    type = "method"
   },
   stroke = {
    description = "For object strokes, Corona uses the concept of paint to a stroke, you control how the boundary is rendered.",
    type = "value"
   },
   strokeWidth = {
    description = "Sets the stroke width of vector objects, in pixels. Note that stroke widths are broken up to inner and outer parts. The stroke is centered on the boundaries of the object, but when the stroke width is odd, Corona does an integer divide by 2 then a math.floor() on the value. For example, `3 / 2 = 1` or `1 / 2 = 0`. The inner width is set to that value, and the remainder is the outer width. This avoids blurring around the edges of the stroke.",
    type = "value"
   }
  },
  description = "Vector display objects are objects created without the use of images, such as rectangles, circles, rounded rectangles, and lines.",
  inherits = "_DisplayObject",
  type = "class"
 },
 _SliderWidget = {
  childs = {
   setValue = {
    args = "( percent )",
    description = "Changes the SliderWidget property.",
    returns = "()",
    type = "method"
   },
   value = {
    description = "A read-only property that represents the current value of the SliderWidget. This is a number between `0` and `100`, representing the percentage at which the slider handle is located.",
    type = "value"
   }
  },
  description = "Slider objects are created using widget.newSlider().",
  inherits = "_GroupObject",
  type = "class"
 },
 _SnapshotObject = {
  childs = {
   canvas = {
    description = "This group is a special offscreen group that enables you to draw on the snapshot's texture without redrawing the objects in snapshot.group.",
    type = "value"
   },
   canvasMode = {
    description = "The canvas mode controls what happens to the children of the snapshot's canvas. ",
    type = "value"
   },
   clearColor = {
    description = "The clear color controls how the snapshot's texture is cleared when the snapshot is invalidated.",
    type = "value"
   },
   group = {
    description = "This group is a special offscreen group that determines what is rendered in the snapshot.",
    type = "value"
   },
   invalidate = {
    args = "()",
    description = "Invalidating snapshots tells Corona to invalidate its texture cache and re-render the children to a texture on the next render pass.",
    returns = "()",
    type = "method"
   },
   textureFilter = {
    description = "This controls the texture filter mode for the snapshot. See the __Texture Keys__ section of display.setDefault() for valid filter values.",
    type = "value"
   },
   textureWrapX = {
    description = "This controls the texture wrap in the __x__ direction for the snapshot. See the __Texture Keys__ section of display.setDefault() for valid wrap values.",
    type = "value"
   },
   textureWrapY = {
    description = "This controls the texture wrap in the __y__ direction for the snapshot. See the __Texture Keys__ section of display.setDefault() for valid wrap values.",
    type = "value"
   }
  },
  description = "Snapshot objects let you capture a group of display objects and render them into a flattened image. ",
  inherits = "_DisplayObject",
  type = "class"
 },
 _SpinnerWidget = {
  childs = {
   start = {
    args = "()",
    description = "This method is used to start the animation or rotation of a SpinnerWidget object.",
    returns = "()",
    type = "method"
   },
   stop = {
    args = "()",
    description = "This method is used to stop the animation or rotation of a SpinnerWidget object.",
    returns = "()",
    type = "method"
   }
  },
  description = "SpinnerWidget objects are created using widget.newSpinner().",
  inherits = "_GroupObject",
  type = "class"
 },
 _SpriteObject = {
  childs = {
   frame = {
    description = "A read-only property that represents the currently shown frame index of loaded sequence.",
    type = "value"
   },
   isPlaying = {
    description = "Returns `true` if the currently loaded sequence is currently playing; `false` if it is not.",
    type = "value"
   },
   numFrames = {
    description = "A read-only property that represents the number of frames in currently loaded sequence.",
    type = "value"
   },
   pause = {
    args = "()",
    description = "Pauses the current animation, and frame remains on the currently shown frame. Playback can resume later with object:play().",
    returns = "()",
    type = "method"
   },
   play = {
    args = "()",
    description = "Play an animation sequence, starting at the current frame. This does not reset looping.",
    returns = "()",
    type = "method"
   },
   sequence = {
    description = "A read-only property that returns the name of the currently playing sequence.",
    type = "value"
   },
   setFrame = {
    args = "( frameIndex )",
    description = "Sets the frame in the currently loaded sequence.",
    returns = "()",
    type = "method"
   },
   setSequence = {
    args = "( [ sequenceName ] )",
    description = "Loads an animation sequence by name.",
    returns = "()",
    type = "method"
   },
   timeScale = {
    description = "Gets or sets the scale to be applied to the animation time. This is used to control a sprite's animation speed dynamically.",
    type = "value"
   }
  },
  description = "Sprite objects are created using display.newSprite().",
  inherits = "_DisplayObject",
  type = "class"
 },
 _StageObject = {
  childs = {
   setFocus = {
    args = "( displayObject [, touchID] )",
    description = "Sets a specific display object events. Pass `nil` to restore default behavior for touch event dispatches.",
    returns = "()",
    type = "method"
   }
  },
  description = "This object is the current \"stage\" object, which is the parent group for all display objects and child groups. This global stage object can be retrieved at any time via display.getCurrentStage().",
  inherits = "_GroupObject",
  type = "class"
 },
 _StepperWidget = {
  childs = {
   getValue = {
    args = "()",
    description = "This method gets the current value of a StepperWidget, this is not restricted to usage within the stepper's listener function.",
    returns = "(Number)",
    type = "method"
   },
   maximumValue = {
    description = "A read-only property that represents the maximum value of the StepperWidget.",
    type = "value"
   },
   minimumValue = {
    description = "A read-only property that represents the minimum value of the StepperWidget.",
    type = "value"
   },
   setValue = {
    args = "( value )",
    description = "This method sets the current value of a StepperWidget. Note that the value passed to this function will not adhere to the stepper's minimum or maximum value. For example, if the stepper has a maximum value of `10` and you pass `20` as the `value` parameter, the stepper's value will be `20`. Thus, you should only pass in a `value` integer that is within range of your stepper's minimum and maximum values, if defined.",
    returns = "()",
    type = "method"
   },
   value = {
    description = "A read-only property that represents the current value of the StepperWidget.",
    type = "value"
   }
  },
  description = "StepperWidget objects are created using widget.newStepper().",
  inherits = "_GroupObject",
  type = "class"
 },
 _SwitchWidget = {
  childs = {
   isOn = {
    description = "The read-only state of a SwitchWidget, either `true` or `false`.",
    type = "value"
   },
   setState = {
    args = "( options )",
    description = "Used to programatically set the state of a SwitchWidget. This also changes the state of the switch visually.",
    returns = "()",
    type = "method"
   }
  },
  description = "SwitchWidget objects are created using widget.newSwitch().",
  inherits = "_GroupObject",
  type = "class"
 },
 _TabBarWidget = {
  childs = {
   setSelected = {
    args = "( buttonIndex )",
    description = "Use this method to set a specific TabBarWidget button to its \"selected\" state. Optionally, you can invoke the `onPress` listener for the button at the same time.",
    returns = "()",
    type = "method"
   }
  },
  description = "TabBarWidget objects are created using widget.newTabBar().",
  inherits = "_GroupObject",
  type = "class"
 },
 _TableViewWidget = {
  childs = {
   deleteAllRows = {
    args = "()",
    description = "This method is used to delete __all__ rows contained inside a TableViewWidget.",
    returns = "()",
    type = "method"
   },
   deleteRow = {
    args = "( rowIndex )",
    description = "This method is used to delete a row contained inside a TableViewWidget.",
    returns = "()",
    type = "method"
   },
   deleteRows = {
    args = "( rowArray [, animationOptions] )",
    description = "This method is used to delete rows contained inside a TableViewWidget.",
    returns = "()",
    type = "method"
   },
   getContentPosition = {
    args = "()",
    description = "Returns the __y__ coordinate of the TableViewWidget content.",
    returns = "(Number)",
    type = "method"
   },
   getNumRows = {
    args = "()",
    description = "Returns the number of rows contained in a TableViewWidget.",
    returns = "(Number)",
    type = "method"
   },
   getRowAtIndex = {
    args = "( rowIndex )",
    description = "Returns the row group reference to a specific __visible__ row in a TableViewWidget.",
    returns = "(GroupObject)",
    type = "method",
    valuetype = "_GroupObject"
   },
   insertRow = {
    args = "( options )",
    description = "This method is used for inserting rows into a TableViewWidget.",
    returns = "()",
    type = "method"
   },
   reloadData = {
    args = "()",
    description = "This method is used to reload <nobr>(re-render)</nobr> the visible rows of a TableViewWidget.",
    returns = "()",
    type = "method"
   },
   scrollToIndex = {
    args = "( rowIndex, time, onComplete )",
    description = "Makes a TableViewWidget scroll to a specific row.",
    returns = "()",
    type = "method"
   },
   scrollToY = {
    args = "( options )",
    description = "Makes a TableViewWidget.",
    returns = "()",
    type = "method"
   },
   setIsLocked = {
    args = "( isLocked )",
    description = "Sets a TableViewWidget to either locked (does not scroll) or unlocked (default behavior).",
    returns = "()",
    type = "method"
   }
  },
  description = "TableViewWidget objects are created using widget.newTableView().",
  inherits = "_GroupObject",
  type = "class"
 },
 _TextBox = {
  childs = {
   align = {
    description = "Alignment of text displayed in the text box. May be set to one of the following strings: ",
    type = "value"
   },
   font = {
    description = "Gets or sets the native text box's font. May be set to a font object as returned by the native.newFont() function.",
    type = "value"
   },
   hasBackground = {
    description = "Controls whether the text box has an opaque background or not. Default is `true` (opaque).",
    type = "value"
   },
   isEditable = {
    description = "Controls whether the text box is editable. Default is `false`.",
    type = "value"
   },
   isFontSizeScaled = {
    description = "Determines which font size units the text box's object.size properties are measured in.",
    type = "value"
   },
   placeholder = {
    description = "Native text boxes can display optional placeholder text (`nil` by default). This can provide a \"hint\" as to what the user should enter in the box. If set, the placeholder string is displayed using the same style information (except the text color). The placeholder does not appear once actual text has been input into the box and it does not currently participate in determining the size of the text box.",
    type = "value"
   },
   setReturnKey = {
    args = "( key )",
    description = "Sets the return key value on the keyboard.",
    returns = "()",
    type = "method"
   },
   setSelection = {
    args = "( startPosition, endPosition )",
    description = "Sets the cursor position if the start and end positions are the same. Alternatively, sets a range of selected text if the start and end positions are different.",
    returns = "()",
    type = "method"
   },
   setTextColor = {
    args = "( r, g, b )",
    description = "Sets the color of the text in a native text input box.",
    returns = "()",
    type = "method"
   },
   size = {
    description = "Gets or sets the native text box's font size.",
    type = "value"
   },
   text = {
    description = "The contents of the native text input box.",
    type = "value"
   }
  },
  description = "A native text box is a scrollable, <nobr>multi-line</nobr> input field for displaying <nobr>text-based</nobr> content. See native.newTextBox() for more details.",
  inherits = "_NativeDisplayObject",
  type = "class"
 },
 _TextField = {
  childs = {
   align = {
    description = "Alignment of text displayed in the input text field. May be set to one of the following strings: ",
    type = "value"
   },
   autocorrectionType = {
    description = "This iOS-only property controls the type of <nobr>auto-correction</nobr> performed. Possible values are:",
    type = "value"
   },
   font = {
    description = "Gets or sets the native text field's font. May be set to a font object as returned by the native.newFont() function.",
    type = "value"
   },
   hasBackground = {
    description = "Controls whether the text box has an opaque background or not. Default is `true` (opaque).",
    type = "value"
   },
   inputType = {
    description = "Sets the keyboard type for a native text input field.",
    type = "value"
   },
   isFontSizeScaled = {
    description = "Determines which font size units the text field's object.size properties are measured in.",
    type = "value"
   },
   isSecure = {
    description = "Controls whether text in the field is obscured. This is useful for passwords. Default is `false`.",
    type = "value"
   },
   placeholder = {
    description = "Native text fields can display optional placeholder text (`nil` by default). This can provide a \"hint\" as to what the user should enter in the field. If set, the placeholder string is displayed using the same style information (except the text color). The placeholder does not appear once actual text has been input into the field and it does not currently participate in determining the size of the text field.",
    type = "value"
   },
   resizeFontToFitHeight = {
    args = "()",
    description = "Changes the font size to \"best fit\" the current height of the text field. This will automatically occur when you first create a text field via the native.newTextField() function.",
    returns = "()",
    type = "method"
   },
   resizeHeightToFitFont = {
    args = "()",
    description = "Changes the height of the text field to \"best fit\" the font size that it's currently using.",
    returns = "()",
    type = "method"
   },
   setReturnKey = {
    args = "( key )",
    description = "Sets the return key value on the keyboard.",
    returns = "()",
    type = "method"
   },
   setSelection = {
    args = "( startPosition, endPosition )",
    description = "Sets the cursor position if the start and end positions are the same. Alternatively, sets a range of selected text if the start and end positions are different.",
    returns = "()",
    type = "method"
   },
   setTextColor = {
    args = "( r, g, b )",
    description = "Sets the color of the text in a native text input field.",
    returns = "()",
    type = "method"
   },
   size = {
    description = "Gets or sets the native text field's font size.",
    type = "value"
   },
   spellCheckingType = {
    description = "This iOS-only property controls the type of spell checking behavior. Possible values are:",
    type = "value"
   },
   text = {
    description = "The contents of the native text input field.",
    type = "value"
   }
  },
  description = "A native text field is a single-line input field for displaying <nobr>text-based</nobr> content. See native.newTextField() for more details.",
  inherits = "_NativeDisplayObject",
  type = "class"
 },
 _TextObject = {
  childs = {
   baselineOffset = {
    description = "The amount by which the baseline of the first line is offset from the center of the text object.",
    type = "value"
   },
   setEmbossColor = {
    args = "( colorTable )",
    description = "Sets the color parameters for an embossed text object created via display.newEmbossedText().",
    returns = "()",
    type = "method"
   },
   size = {
    description = "Retrieves or sets the text size pertaining to a text object.",
    type = "value"
   },
   text = {
    description = "Retrieves or sets the text string of a text object.",
    type = "value"
   }
  },
  description = "Text objects are created using the display.newText() function.",
  inherits = "_DisplayObject",
  type = "class"
 },
 _TextureResource = {
  childs = {
   baseDir = {
    description = "Use this property instead of the real directory constant to create display objects, fills, and use in other places where a real directory constant is expected. In these instances, you must also pass texture.filename as a parameter - see the examples below for reference.",
    type = "value"
   },
   filename = {
    description = "Use this property instead of the real file name to create display objects, fills, and use in other places where a real image file name is expected. In these instances, you must also pass texture.baseDir as a parameter - see the examples below for reference.",
    type = "value"
   },
   releaseSelf = {
    args = "()",
    description = "The graphics.newTexture() object and restore automatic management for the texture, you must release it by calling `texture:releaseSelf()`.",
    returns = "()",
    type = "method"
   },
   type = {
    description = "This property indicates the type of texture.",
    type = "value"
   }
  },
  description = "The base class for all textures created by graphics.newTexture(). Objects of this type provide a handle to manually manage and access information about textures.",
  type = "class"
 },
 _TextureResourceBitmap = {
  childs = {
   preload = {
    args = "()",
    description = "When an image texture is created, the bitmap is read from the file system and loaded into memory. At the first point a display object is created which utilizes the texture, it is scheduled to load to the GPU.",
    returns = "()",
    type = "method"
   },
   wrapX = {
    description = "Indicates the horizontal wrap mode for a texture: `\"clampToEdge\"`, `\"repeat\"` or `\"mirroredRepeat\"`.",
    type = "value"
   },
   wrapY = {
    description = "Indicates the vertical wrap mode for a texture: `\"clampToEdge\"`, `\"repeat\"` or `\"mirroredRepeat\"`.",
    type = "value"
   }
  },
  description = "The object created by graphics.newTexture()",
  inherits = "_TextureResource",
  type = "class"
 },
 _TextureResourceCanvas = {
  childs = {
   anchorX = {
    description = "This property can be used to change the origin point of the rendered canvas. Anchor point units are relative which means that setting `anchorX` to `1.0` will offset contents of the canvas by texture.width. Default is `0.0`.",
    type = "value"
   },
   anchorY = {
    description = "This property can be used to change the origin point of the rendered canvas. Anchor point units are relative which means that setting `anchorY` to `1.0` will offset contents of the canvas by texture.height. Default is `0.0`.",
    type = "value"
   },
   cache = {
    description = "After being rendered to the TextureResourceCanvas is moved to this group.",
    type = "value"
   },
   draw = {
    args = "( object )",
    description = "This method adds/renders a display object.",
    returns = "()",
    type = "method"
   },
   height = {
    description = "This property indicates the height in which objects can be rendered within a TextureResourceCanvas. Essentially, if a display object drawn to the canvas is equal to this height, it will occupy the entire vertical space.",
    type = "value"
   },
   invalidate = {
    args = "( [params] )",
    description = "This method schedules display objects queued for rendering via texture:draw() to be rendered to the texture before the next frame.",
    returns = "()",
    type = "method"
   },
   pixelHeight = {
    description = "This read-only property indicates the vertical pixel dimensions of the texture that the canvas resource is rendered to.",
    type = "value"
   },
   pixelWidth = {
    description = "This read-only property indicates the horizontal pixel dimensions of the texture that the canvas resource is rendered to.",
    type = "value"
   },
   setBackground = {
    args = "( gray )",
    description = "Sets the fill color of the texture, also known as the \"clear color\" or the background. The entire texture will be set to the specified color during the next scheduled <nobr>non-accumulative</nobr> rendering. Default is transparent.",
    returns = "()",
    type = "method"
   },
   width = {
    description = "This read-only property indicates the width in which objects can be rendered within a TextureResourceCanvas. Essentially, if a display object drawn to the canvas is equal to this width, it will occupy the entire horizontal space.",
    type = "value"
   }
  },
  description = "The object created by graphics.newTexture()(https://en.wikipedia.org/wiki/Render_Target).",
  inherits = "_TextureResource",
  type = "class"
 },
 _TextureResourceExternal = {
  childs = {
   height = {
    description = "This read-only property indicates the vertical pixel dimensions of the texture.",
    type = "value"
   },
   invalidate = {
    args = "()",
    description = "Calling this function will update the texture from the source provided by a plugin.",
    returns = "()",
    type = "method"
   },
   width = {
    description = "This read-only property indicates the horizontal pixel dimensions of the texture.",
    type = "value"
   }
  },
  description = "The object created by various plugins, with a type property of `\"external\"`.",
  inherits = "_TextureResource",
  type = "class"
 },
 _Video = {
  childs = {
   currentTime = {
    description = "The current time position of the video in seconds (read-only). Use this in conjunction with native.newVideo().",
    type = "value"
   },
   isMuted = {
    description = "Controls whether the video's audio channel is muted. Use this in conjunction with native.newVideo().",
    type = "value"
   },
   load = {
    args = "( path )",
    description = "Loads a specified video. Use this in conjunction with native.newVideo().",
    returns = "()",
    type = "method"
   },
   pause = {
    args = "()",
    description = "Pauses the currently loaded video. Use this in conjunction with native.newVideo().",
    returns = "()",
    type = "method"
   },
   play = {
    args = "()",
    description = "Plays the currently loaded video. Use this in conjunction with native.newVideo().",
    returns = "()",
    type = "method"
   },
   seek = {
    args = "( timeInSeconds )",
    description = "Seeks and jumps to the specified time in the currently loaded video. Use this in conjunction with native.newVideo().",
    returns = "()",
    type = "method"
   },
   totalTime = {
    description = "The read-only length of the currently loaded video, in seconds.",
    type = "value"
   }
  },
  description = "A video object is a native display object.",
  inherits = "_NativeDisplayObject",
  type = "class"
 },
 _WebView = {
  childs = {
   back = {
    args = "()",
    description = "Takes the webView back one step in the WebView history.",
    returns = "()",
    type = "method"
   },
   canGoBack = {
    description = "Read-only property that will be `true` if the WebView can go back a web page or `false` if not.",
    type = "value"
   },
   canGoForward = {
    description = "Read-only property that will be `true` if the WebView can go forward a web page or `false` if not.",
    type = "value"
   },
   deleteCookies = {
    args = "()",
    description = "Deletes cookies for the WebView.",
    returns = "()",
    type = "method"
   },
   forward = {
    args = "()",
    description = "Takes the WebView forward one step in its history.",
    returns = "()",
    type = "method"
   },
   hasBackground = {
    description = "This is a get/set property that corresponds to whether or not the background of the WebView is visible or transparent. Default is `true`.",
    type = "value"
   },
   reload = {
    args = "()",
    description = "Reloads the currently loaded page in a WebView.",
    returns = "()",
    type = "method"
   },
   request = {
    args = "( url )",
    description = "Loads the specified URL (string) into the WebView directory as a search path.",
    returns = "()",
    type = "method"
   },
   stop = {
    args = "()",
    description = "Stops loading the current page in the WebView (if loading).",
    returns = "()",
    type = "method"
   }
  },
  description = "A web view object is a native display object.",
  inherits = "_NativeDisplayObject",
  type = "class"
 },
 ads = {
  childs = {},
  description = "Corona supports a variety of ad provider plugins for <nobr>in-app</nobr> advertising. Please see __Ads/Monetization__ in the Plugins directory for options.",
  type = "lib"
 },
 audio = {
  childs = {
   dispose = {
    args = "( audioHandle )",
    description = "Releases audio memory associated with a handle.",
    returns = "()",
    type = "function"
   },
   fade = {
    args = "( [ { [channel=c] [, time=t] [, volume=v ] } ] )",
    description = "This fades a playing sound in a specified amount to a specified volume. The audio will continue playing after the fade completes.",
    returns = "(Number)",
    type = "function"
   },
   fadeOut = {
    args = "( [ { [channel=c] [, time=t] } ] )",
    description = "This stops a playing sound in a specified amount of time and fades to min volume while doing it. The audio will stop at the end of the time and the channel will be freed.",
    returns = "(Number)",
    type = "function"
   },
   findFreeChannel = {
    args = "( [ startChannel ] )",
    description = "Looks for an available audio channel for playback. You can provide a start channel number as parameter and begin searching from that channel, increasing upward to the highest channel. The search does not include reserved channels.",
    returns = "(Number)",
    type = "function"
   },
   freeChannels = {
    description = "This property is equal to the number of channels that are currently available for playback (channels not playing or paused).",
    type = "value"
   },
   getDuration = {
    args = "( audioHandle )",
    description = "This function returns the total time in milliseconds of the audio resource. If the total length cannot be determined, `-1` will be returned.",
    returns = "(Number)",
    type = "function"
   },
   getMaxVolume = {
    args = "( { channel=c } )",
    description = "Gets the max volume for a specific channel. There is no max volume for the master volume.",
    returns = "(Number)",
    type = "function"
   },
   getMinVolume = {
    args = "( { channel=c } )",
    description = "Gets the minimum volume for a specific channel. There is no minimum volume for the master volume.",
    returns = "(Number)",
    type = "function"
   },
   getVolume = {
    args = "( { channel=c } )",
    description = "Gets the volume for a specific channel, or gets the master volume.",
    returns = "(Number)",
    type = "function"
   },
   isChannelActive = {
    args = "( channel )",
    description = "Returns `true` if the specified channel is currently playing or paused; `false` if otherwise.",
    returns = "(Boolean)",
    type = "function"
   },
   isChannelPaused = {
    args = "( channel )",
    description = "Returns `true` if the specified channel is currently paused; `false` if not.",
    returns = "(Boolean)",
    type = "function"
   },
   isChannelPlaying = {
    args = "( channel )",
    description = "Returns `true` if the specified channel is currently playing; `false` if otherwise.",
    returns = "(Boolean)",
    type = "function"
   },
   loadSound = {
    args = "( audiofileName [, baseDir ]  )",
    description = "Loads an entire file completely into memory and returns a reference to the audio data. Files that are loaded completely into memory may be reused/played/shared simultaneously on multiple channels so you only need to load one instance of the file. You should use this to load all your short sounds, especially ones you may play frequently. For best results, load all the sounds at the launch of your app or the start of a new level/scene.",
    returns = "(Table)",
    type = "function"
   },
   loadStream = {
    args = "( audioFileName [, baseDir ]  )",
    description = "Loads (opens) a file to be read as streaming audio. Streamed files are read in little chunks at a time to minimize memory use. These are intended for large/long files like background music and speech. Unlike files loaded with audio.loadSound(), these cannot be shared simultaneously across multiple channels. If you need to play multiple simulataneous instances of the same file, you must load multiple instances of the file.",
    returns = "(Object)",
    type = "function"
   },
   pause = {
    args = "( [channel] )",
    description = "Pauses playback on a channel (or all channels if no channels are specified). Has no effect on channels that aren't playing.",
    returns = "(Number)",
    type = "function"
   },
   play = {
    args = "( audioHandle [, options ] )",
    description = "Plays the audio specified by the audio handle on a channel. If a channel is not explicitly specified, an available channel will be automatically chosen for you.",
    returns = "(Number)",
    type = "function"
   },
   reserveChannels = {
    args = "( channels )",
    description = "Allows you to reserve a certain number of channels so they won't be automatically assigned to play on. This function blocks off the lower number channels up to the number you specify so they won't be automatically assigned to be played on when you call various play functions.",
    returns = "(Number)",
    type = "function"
   },
   reservedChannels = {
    description = "Returns the number of reserved channels set by audio.reserveChannels().",
    type = "value"
   },
   resume = {
    args = "( [channel] )",
    description = "Resumes playback on a channel that is paused (or all channels if no channel is specified). Should have no effect on channels that aren't paused.",
    returns = "(Number)",
    type = "function"
   },
   rewind = {
    args = "( [audioHandle | options] )",
    description = "Rewinds audio to the beginning position on either an active channel or directly on the audio handle (rewinds all channels if no arguments are specified).",
    returns = "(Boolean)",
    type = "function"
   },
   seek = {
    args = "( time [, audioHandle ] [, options ] )",
    description = "Seeks to a time position on either an active channel or directly on the audio handle.",
    returns = "(Boolean)",
    type = "function"
   },
   setMaxVolume = {
    args = "( volume, options )",
    description = "Clamps the maximum volume to the set value. Any volumes that exceed the value will be played at the maximum volume level.",
    returns = "(Boolean)",
    type = "function"
   },
   setMinVolume = {
    args = "( volume, options )",
    description = "Clamps the minimum volume to the set value. Any volumes that fall below the value will be played at the minimum volume level.",
    returns = "(Boolean)",
    type = "function"
   },
   setVolume = {
    args = "( volume [, options ] )",
    description = "Sets the volume either for a specific channel, or sets the master volume.",
    returns = "(Boolean)",
    type = "function"
   },
   stop = {
    args = "( [channel] )",
    description = "Stops playback on a channel <nobr>(or all channels)</nobr> and clears the channel(s) so they can be played on again. Callbacks will still be invoked, but the completed flag will be set to `false`.",
    returns = "(Number)",
    type = "function"
   },
   stopWithDelay = {
    args = "( duration [, options ] )",
    description = "Stops a currently playing sound after the specified time delay.",
    returns = "(Number)",
    type = "function"
   },
   totalChannels = {
    description = "Returns the total number of channels. This value is currently 32, but the limit may change in the future.",
    type = "value"
   },
   unreservedFreeChannels = {
    description = "Returns the number of channels that are currently available for playback (channels not playing or paused), excluding the channels which have been reserved.",
    type = "value"
   },
   unreservedUsedChannels = {
    description = "Returns the number of channels that are currently in use (playing or paused), excluding the channels which have been reserved.",
    type = "value"
   },
   usedChannels = {
    description = "Returns the number of channels that are currently in use (playing or paused).",
    type = "value"
   }
  },
  description = "The Corona Audio system gives you access to advanced OpenAL features. See the Audio Usage/Functions guide for more information.",
  type = "lib"
 },
 composer = {
  childs = {
   getScene = {
    args = "( sceneName )",
    description = "Returns the specified scene object, as returned from composer.newScene(). Returns `nil` if the scene object does not exist. This function is useful for getting a reference to a specific scene object - for instance, if the current scene needs access to a specific function that's attached to another scene.",
    returns = "(Table)",
    type = "function"
   },
   getSceneName = {
    args = "( sceneType )",
    description = "Returns the current, previous, or overlay scene name as a string. This can then be used with composer.gotoScene().",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   getVariable = {
    args = "( variableName )",
    description = "Retrieves the value of a variable from within the Composer module that was previously set via composer.setVariable().",
    returns = "()",
    type = "function"
   },
   gotoScene = {
    args = "( sceneName [, options] )",
    description = "This function is used to transition to a specific scene. When called, the hide event will be dispatched.",
    returns = "()",
    type = "function"
   },
   hideOverlay = {
    args = "( [recycleOnly] [, effect] [, time] )",
    description = "This function hides/removes the current overlay scene (see composer.showOverlay()), if one is being displayed.",
    returns = "()",
    type = "function"
   },
   isDebug = {
    description = "Toggles \"Composer Debug Mode\" which, if set to `true`, prints useful debugging information to the Corona Simulator Console in certain situations. This should be set to `false` (default) before building the project for deployment.",
    type = "value"
   },
   loadScene = {
    args = "( sceneName [, doNotLoadView] [, params] )",
    description = "Loads the specified scene, hidden behind the current scene, without initiating a scene transition. This function is similar to composer.gotoScene() event will be dispatched on the scene, assuming its `self.view` does not already exist.",
    returns = "()",
    type = "function"
   },
   newScene = {
    args = "()",
    description = "This function creates new scene objects which can be used with the Composer Library.",
    returns = "(Table)",
    type = "function"
   },
   recycleAutomatically = {
    description = "If this property is set to `true` (default), the scene used least recently will be automatically recycled if the OS issues a low memory warning. If you prefer to manage the recycling of scenes manually and disable the <nobr>auto-recycling</nobr> functionality, set this property to `false`.",
    type = "value"
   },
   recycleOnLowMemory = {
    description = "If the OS issues a low memory warning, Composer will automatically recycle the scene used least recently (that scene's `self.view` will be removed). If you prefer to disable <nobr>auto-recycling</nobr> functionality, set this property to `false`. Default is `true`.",
    type = "value"
   },
   recycleOnSceneChange = {
    description = "By default, when changing scenes, Composer keeps the current scene's view (`self.view`) in memory, which can improve performance if you access the same scenes frequently. If you set `composer.recycleOnSceneChange` to `true`, the scene's `self.view` display group will be removed, but its scene object will remain in memory. The default value is `false`.",
    type = "value"
   },
   removeHidden = {
    args = "( [shouldRecycle] )",
    description = "This function removes (or optionally recycles) all scenes __except__ for the currently active scene. A destroy event is first dispatched to all of these scenes.",
    returns = "()",
    type = "function"
   },
   removeScene = {
    args = "( sceneName [, shouldRecycle] )",
    description = "This function removes the specified scene (or optionally recycles it). A destroy event is first dispatched to the scene.",
    returns = "()",
    type = "function"
   },
   setVariable = {
    args = "( variableName, value )",
    description = "This function sets a variable within the Composer module which you can access from any other scene via  composer.getVariable().",
    returns = "()",
    type = "function"
   },
   showOverlay = {
    args = "( sceneName [, options] )",
    description = "This function loads an overlay scene above the currently active scene - <nobr>the \"parent\" scene -</nobr> leaving the parent scene intact.",
    returns = "()",
    type = "function"
   },
   stage = {
    description = "Returns a reference to the top-level Composer display group which all scene views are inserted into. This can be considered as the \"Composer scene layer\" and it's useful if you need to place display objects above or below all Composer scenes, even during transition effects. For example:",
    type = "value"
   }
  },
  description = "Composer is the official scene (screen) creation and management library in CORONA_CORE_PRODUCT. This library provides developers with an easy way to create and transition between individual scenes.",
  type = "lib"
 },
 crypto = {
  childs = {
   digest = {
    args = "( algorithm, data [, raw] )",
    description = "Generates the message digest (the hash) of the input string.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   hmac = {
    args = "( algorithm, data, key [, raw] )",
    description = "Computes HMAC (Key-Hashing for Message Authentication Code) of the string and returns it.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   md4 = {
    description = "Constant used to specify the MD4 algorithm (Message-Digest algorithm 4).",
    type = "value"
   },
   md5 = {
    description = "Constant used to specify the MD5 algorithm (Message-Digest algorithm 5).",
    type = "value"
   },
   sha1 = {
    description = "Constant used to specify the SHA-1 algorithm.",
    type = "value"
   },
   sha224 = {
    description = "Constant used to specify the SHA-224 algorithm.",
    type = "value"
   },
   sha256 = {
    description = "Constant used to specify the SHA-256 algorithm.",
    type = "value"
   },
   sha384 = {
    description = "Constant used to specify the SHA-384 algorithm.",
    type = "value"
   },
   sha512 = {
    description = "Constant used to specify the SHA-512 algorithm.",
    type = "value"
   }
  },
  description = "Corona provides routines for calculating common message digests (hashes) and hash-based message authentication codes (HMAC).",
  type = "lib"
 },
 display = {
  childs = {
   actualContentHeight = {
    description = "The height in Corona content units of the screen. The result depends on the `scale` setting in `config.lua`. See the Project Configuration guide for more information.",
    type = "value"
   },
   actualContentWidth = {
    description = "The width in Corona content units of the screen. The result depends on the `scale` setting in `config.lua`. See the Project Configuration guide for more information.",
    type = "value"
   },
   capture = {
    args = "( displayObject, options )",
    description = "This function is the same as display.save(), but it returns a display object instead of saving to a file by default. You can optionally save the capture to the device's photo library, but this is not the default action - you must explicitly tell it to do so when calling the function.",
    returns = "(DisplayObject)",
    type = "function",
    valuetype = "_DisplayObject"
   },
   captureBounds = {
    args = "( screenBounds [, saveToPhotoLibrary] )",
    description = "Captures a portion of the screen and returns it as a new DisplayObject. You can specify what portion of the screen to capture by passing in rectangular bounds. You can optionally save the capture image as a file to the device's photo library.",
    returns = "(DisplayObject)",
    type = "function",
    valuetype = "_DisplayObject"
   },
   captureScreen = {
    args = "( [saveToPhotoLibrary] )",
    description = "Captures the contents of the screen and returns it as a new display object. You can optionally save the capture as a file to the device's photo library.",
    returns = "(DisplayObject)",
    type = "function",
    valuetype = "_DisplayObject"
   },
   colorSample = {
    args = "( x, y, listener )",
    description = "Get the color of a pixel on screen.",
    returns = "()",
    type = "function"
   },
   contentCenterX = {
    description = "The center of the content area along the __x__ axis. Equivalent to `display.contentWidth * 0.5`.",
    type = "value"
   },
   contentCenterY = {
    description = "The center of the content area along the __y__ axis. Equivalent to `display.contentHeight * 0.5`.",
    type = "value"
   },
   contentHeight = {
    description = "A read-only property representing the height of the content area in pixels. This will default to the screen height, but may be another value if you are using content scaling in `config.lua`.",
    type = "value"
   },
   contentScaleX = {
    description = "The ratio between content pixel and screen pixel width.",
    type = "value"
   },
   contentScaleY = {
    description = "The ratio between content pixel and screen pixel height.",
    type = "value"
   },
   contentWidth = {
    description = "A read-only property representing the width of the content area in pixels. This will default to the screen width, but may be another value if you are using content scaling in `config.lua`.",
    type = "value"
   },
   currentStage = {
    description = "A reference to the current stage object, which is the root group for all display objects and groups. Currently, Corona has a single stage instance, so this function always returns a reference to the same object.",
    type = "value"
   },
   fps = {
    description = "Current framerate of the running application.",
    type = "value"
   },
   getCurrentStage = {
    args = "()",
    description = "Returns a reference to the current stage object, which is the parent group for all display objects and groups. Currently, Corona has a single stage instance, so this function always returns a reference to the same object.",
    returns = "(DisplayObject)",
    type = "function",
    valuetype = "_DisplayObject"
   },
   getDefault = {
    args = "( key )",
    description = "Get default display values including default colors for display objects, anchor point defaults, texture wrapping settings, etc.",
    returns = "()",
    type = "function"
   },
   getSafeAreaInsets = {
    args = "()",
    description = "Returns four numbers corresponding to the top, left, bottom, and right \"safe area\" insets. The \"safe area\" is the rectangular region where it's safe to place important UI elements, ensuring that they are not obscured by status bars, device aspects like a sensor bar or rounded corners, software buttons, TV overscan areas, etc.",
    returns = "(Numbers)",
    type = "function"
   },
   imageSuffix = {
    description = "Returns the suffix used by the dynamic image selection feature of Corona. If the content scaling is 1, then returns `nil`.",
    type = "value"
   },
   loadRemoteImage = {
    args = "( url, method, listener [, params], destFilename [, baseDir] [, x, y] )",
    description = "This a convenience method, similar to network.download() containing the image, as well as saving the image to a file.",
    returns = "()",
    type = "function"
   },
   newCircle = {
    args = "( [parent,] xCenter, yCenter, radius )",
    description = "Creates a circle with radius centered at specified coordinates (`xCenter`, `yCenter`). The local origin is at the center of the circle and the anchor point is initialized to this local origin.",
    returns = "(ShapeObject)",
    type = "function",
    valuetype = "_ShapeObject"
   },
   newContainer = {
    args = "( [parent,] width, height )",
    description = "Containers are a special kind of group in which the children are clipped (masked) by the bounds of the container.",
    returns = "()",
    type = "function"
   },
   newEmbossedText = {
    args = "( options )",
    description = "Creates an embossed text object. The local origin is at the center of the text and the anchor point is initialized to this local origin.",
    returns = "(TextObject)",
    type = "function",
    valuetype = "_TextObject"
   },
   newEmitter = {
    args = "( emitterParams [, baseDir] )",
    description = "This function creates an EmitterObject, used to display particle effects.",
    returns = "(EmitterObject)",
    type = "function",
    valuetype = "_EmitterObject"
   },
   newGroup = {
    args = "()",
    description = "Creates a display group for organizing/layering display objects. Initially, there are no children in a group. The local origin is at the parent's origin; the anchor point is initialized to this local origin.",
    returns = "(GroupObject)",
    type = "function",
    valuetype = "_GroupObject"
   },
   newImage = {
    args = "()",
    description = "Displays an image on the screen from a file (supports tinting via object:setFillColor is initialized to this local origin.",
    returns = "(DisplayObject)",
    type = "function",
    valuetype = "_DisplayObject"
   },
   newImageRect = {
    args = "()",
    description = "Displays an image on the screen from a file. This image supports tinting via object:setFillColor is initialized to this local origin.",
    returns = "(DisplayObject)",
    type = "function",
    valuetype = "_DisplayObject"
   },
   newLine = {
    args = "( [parent,] x1, y1, x2, y2 [, x3, y3, ... ] )",
    description = "Draw a line from one point to another. Optionally, you may append points to the end of the line to create outline shapes or paths.",
    returns = "(LineObject)",
    type = "function",
    valuetype = "_LineObject"
   },
   newMesh = {
    args = "( [parent,] [x, y,] options )",
    description = "Draws a mesh shape by providing vertices of the shape. The local origin is at the center of the mesh and the anchor point is initialized to this local origin.",
    returns = "(ShapeObject)",
    type = "function",
    valuetype = "_ShapeObject"
   },
   newPolygon = {
    args = "( [parent,] x, y, vertices )",
    description = "Draws a polygon shape by providing the outline (contour) of the shape. This includes convex or concave shapes. Self-intersecting shapes, however, are not supported and will result in undefined behavior.",
    returns = "(ShapeObject)",
    type = "function",
    valuetype = "_ShapeObject"
   },
   newRect = {
    args = "( [parent,] x, y, width, height )",
    description = "Creates a rectangle object. The local origin is at the center of the rectangle and the anchor point is initialized to this local origin.",
    returns = "(ShapeObject)",
    type = "function",
    valuetype = "_ShapeObject"
   },
   newRoundedRect = {
    args = "( [parent,] x, y, width, height, cornerRadius )",
    description = "Creates a rounded rectangle object. The corners are rounded by quarter circles of a specified radius value. The local origin is at the center of the rectangle and the anchor point is initialized to this local origin.",
    returns = "(ShapeObject)",
    type = "function",
    valuetype = "_ShapeObject"
   },
   newSnapshot = {
    args = "( [parent,] w, h )",
    description = "Snapshot objects let you capture a group of display objects and render them into a flattened image. The image is defined by objects that are added to the snapshot's group property.",
    returns = "(SnapshotObject)",
    type = "function",
    valuetype = "_SnapshotObject"
   },
   newSprite = {
    args = "( [parent,] imageSheet, sequenceData )",
    description = "Creates a sprite object. Sprites allow for animated sequences of frames that reside on __image sheets__. Please see the Sprite Animation and Image Sheets guides for more information.",
    returns = "(SpriteObject)",
    type = "function",
    valuetype = "_SpriteObject"
   },
   newText = {
    args = "( options )",
    description = "Creates a text object is initialized to this local origin.",
    returns = "(TextObject)",
    type = "function",
    valuetype = "_TextObject"
   },
   pixelHeight = {
    description = "The height of the screen for mobile device apps, or the height of the window for desktop apps. This value is in pixels and is always relative to portrait orientation.",
    type = "value"
   },
   pixelWidth = {
    description = "The width of the screen for mobile device apps, or the width of the window for desktop apps. This value is in pixels and is always relative to portrait orientation.",
    type = "value"
   },
   remove = {
    args = "( object )",
    description = "Removes an object or group if not `nil`.",
    returns = "()",
    type = "function"
   },
   safeActualContentHeight = {
    description = "The height of the screen \"safe area\" in Corona content units. The \"safe area\" is the rectangular region where it's safe to place important UI elements, ensuring that they are not obscured by status bars, device aspects like a sensor bar or rounded corners, software buttons, TV overscan areas, etc.",
    type = "value"
   },
   safeActualContentWidth = {
    description = "The width of the screen \"safe area\" in Corona content units. The \"safe area\" is the rectangular region where it's safe to place important UI elements, ensuring that they are not obscured by status bars, device aspects like a sensor bar or rounded corners, software buttons, TV overscan areas, etc.",
    type = "value"
   },
   safeScreenOriginX = {
    description = "Returns the __x__ distance from the left side of the \"safe area\" boundary to the left side of the content area, in Corona content units. The \"safe area\" is the rectangular region where it's safe to place important UI elements, ensuring that they are not obscured by status bars, device aspects like a sensor bar or rounded corners, software buttons, TV overscan areas, etc.",
    type = "value"
   },
   safeScreenOriginY = {
    description = "Returns the __y__ distance from the top side of the \"safe area\" boundary to the top side of the content area, in Corona content units. The \"safe area\" is the rectangular region where it's safe to place important UI elements, ensuring that they are not obscured by status bars, device aspects like a sensor bar or rounded corners, software buttons, TV overscan areas, etc.",
    type = "value"
   },
   save = {
    args = "( displayObject, options )",
    description = "Renders the display object referenced by first argument into a JPEG or PNG image and saves it as a new file. The display object must currently be in the display hierarchy or no image will be saved. If the object is a display group, all children are also rendered.",
    returns = "()",
    type = "function"
   },
   screenOriginX = {
    description = "Returns the __x__ distance from the left side of the actual screen to the left side of the content area, in Corona content units. For example, in `\"letterbox\"` or `\"zoomEven\"` scaling modes, there may be added area or cropped area on the current device screen. This lets you find out how much visible area has been added or removed on the current device.",
    type = "value"
   },
   screenOriginY = {
    description = "Returns the __y__ distance from the top of the actual screen to the top of the content area, in Corona content units. For example, in `\"letterbox\"` or `\"zoomEven\"` scaling modes, there may be added area or cropped area on the current device screen. This lets you find out how much visible area has been added or removed on the current device.",
    type = "value"
   },
   setDefault = {
    args = "()",
    description = "Set default display values including default colors for display objects, anchor point defaults, texture wrapping settings, etc.",
    returns = "()",
    type = "function"
   },
   setDrawMode = {
    args = "( key, value )",
    description = "Sets the draw mode.",
    returns = "()",
    type = "function"
   },
   setStatusBar = {
    args = "( mode )",
    description = "Hides or changes the appearance of the status bar on most devices.",
    returns = "()",
    type = "function"
   },
   statusBarHeight = {
    description = "A read-only property representing the height of the status bar in content pixels.",
    type = "value"
   },
   topStatusBarContentHeight = {
    description = "A read-only property representing the height of the status bar in content coordinates.",
    type = "value"
   },
   viewableContentHeight = {
    description = "A read-only property that contains the height of the viewable screen area in content coordinates. ",
    type = "value"
   },
   viewableContentWidth = {
    description = "A read-only property that contains the width of the viewable screen area in content coordinates. ",
    type = "value"
   }
  },
  type = "lib"
 },
 easing = {
  childs = {},
  description = "Easing functions provide a simple way of interpolating between two values to achieve varied animations. They are used in conjunction with the transition library.",
  type = "lib"
 },
 event = {
  childs = {},
  description = "",
  type = "lib"
 },
 facebook = {
  childs = {},
  description = "The `facebook` library provides access to Facebook Connect, a set of web APIs for accessing the Facebook social network. The functions allow a user to login/logout, post messages/images, and retrieve status.",
  type = "lib"
 },
 gameNetwork = {
  childs = {},
  description = "Corona's `gameNetwork` library provides access to social gaming features such as public leaderboards and achievements.",
  type = "lib"
 },
 graphics = {
  childs = {
   defineEffect = {
    args = "( effect )",
    description = "This function allows you to extend Corona and define a custom shader effect. Your custom effect can define either a vertex kernel or a fragment kernel (or both). These kernels are similar to shaders, except that they must define functions with a specific name and which conform to specific function signatures. ",
    returns = "()",
    type = "function"
   },
   getFontMetrics = {
    args = "( fontName [, fontSize] )",
    description = "This function calculates and returns the various metrics for the font with the given name (file name) and text size. The returned value is a table containing the following properties:",
    returns = "(Table)",
    type = "function"
   },
   newImageSheet = {
    args = "( filename, [baseDir, ] options )",
    description = "ImageSheet and they can also be used to preload many static images.",
    returns = "(ImageSheet)",
    type = "function",
    valuetype = "_ImageSheet"
   },
   newMask = {
    args = "( filename [, baseDir] )",
    description = "Creates a bit mask from an image file. The image is converted internally to grayscale. The white pixels of the bit mask allow the covered image to be visible, while black pixels hide (mask) the covered image. The area outside of the mask must be filled with black pixels which mask off the area outside the mask image.",
    returns = "(Mask)",
    type = "function",
    valuetype = "_Mask"
   },
   newOutline = {
    args = "()",
    description = "This function produces the outline of the shape obtained from an image file or a frame within an ImageSheet. A shape is recognized by the image's <nobr>non-transparent</nobr> alpha value. Generally, simpler images produce better outlines.",
    returns = "(Table)",
    type = "function"
   },
   newTexture = {
    args = "( params )",
    description = "Creates a TextureResource object which allows you to access and manage textures. This can be used to <nobr>pre-load</nobr> a texture/image and prevent it from being disposed when there is no display object using it.",
    returns = "(TextureResource)",
    type = "function",
    valuetype = "_TextureResource"
   },
   releaseTextures = {
    args = "( params )",
    description = "This convenience function disposes/releases all TextureResource.",
    returns = "(TextureResource)",
    type = "function",
    valuetype = "_TextureResource"
   }
  },
  type = "lib"
 },
 io = {
  childs = {
   close = {
    args = "( [file] )",
    description = "Closes an open file handle. Equivalent to `file:close()`. If a file handle is not specified, this function closes the default output file.",
    returns = "()",
    type = "function"
   },
   flush = {
    args = "()",
    description = "Flushes the default output file. Equivalent to `io.output():flush`.",
    returns = "()",
    type = "function"
   },
   input = {
    args = "( file )",
    description = "Sets the standard input file. When called with a file handle, it simply sets this file handle as the default input file. When called without parameters, it returns the current default input file.",
    returns = "(Object)",
    type = "function"
   },
   lines = {
    args = "( file )",
    description = "Opens the given file name in read mode and returns an iterator function that, each time it is called, returns a new line from the file.",
    returns = "(Function)",
    type = "function"
   },
   open = {
    args = "( file [, mode] )",
    description = "This function opens a file for reading or writing, in the string (default) or binary mode. It returns a new file handle or, in case of errors, `nil` plus an error message. This function can also be used to create a new file.",
    returns = "(Object)",
    type = "function"
   },
   output = {
    args = "( [file] )",
    description = "Sets the standard output file. When called with a file name, it opens the named file in text mode and sets its handle as the default output file. When called with a file handle, it simply sets this file handle as the default output file. When called without parameters, it returns the current default output file.",
    returns = "(Object)",
    type = "function"
   },
   read = {
    args = "( [fmt1] [, fmt2] [, ...] )",
    description = "Reads the file set by io.input() with the characters read, or `nil` if it cannot read data with the specified format. When called without formats, it uses a default format that reads the entire next line (`\"*l\"`).",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   tmpfile = {
    args = "()",
    description = "Opens a temporary file for reading and writing and returns a handle to it. When the app ends normally, this file will be deleted.",
    returns = "(Object)",
    type = "function"
   },
   type = {
    args = "( obj )",
    description = "Checks whether `obj` is a valid file handle. Returns the string `\"file\"` if `obj` is an open file handle, `\"closed file\"` if `obj` is a closed file handle, or `nil` if `obj` is not a file handle.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   write = {
    args = "( arg1 [, arg2] [, ...] )",
    description = "Writes the value of each of its arguments to the file. The arguments must be strings or numbers. To write other values, use tostring() before writing.",
    returns = "()",
    type = "function"
   }
  },
  description = "Standard Lua library to create, write, and read files.",
  type = "lib"
 },
 json = {
  childs = {
   decode = {
    args = "( data [, position [, nullval]] )",
    description = "Decodes the JSON-encoded data structure and returns a Lua object (table) with the data. The return value is a Lua object when the data is successfully decoded or, in the case of an error, three values: `nil`, the position of the next character that doesn't belong to the object, and an error message.",
    returns = "(Table)",
    type = "function"
   },
   decodeFile = {
    args = "( filename [, position [, nullval]] )",
    description = "Decodes the contents of a file that is expected to contain <nobr>JSON-encoded</nobr> data and returns a Lua object (table) with the appropriate data. The return value is a Lua object when the data is successfully decoded or, in the case of an error, three values: `nil`, the position of the next character that doesn't belong to the object, and an error message. Errors can either relate to issues opening the file or to JSON syntax issues.",
    returns = "(Table)",
    type = "function"
   },
   encode = {
    args = "( t [, options] )",
    description = "Returns the Lua object (table) as a JSON-encoded string. Since items with `nil` values in a Lua table effectively don't exist, you should use `json.null` as a placeholder value if you need to preserve array indices in your JSON (see discussion of `nullval` in json.decode()).",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   prettify = {
    args = "( obj )",
    description = "Returns a string which is a human readable representation of the given object (or valid JSON string). The string is indented and the top-level keys are sorted into alphabetical order suitable for displaying on the console with `print()`.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   }
  },
  description = "The `json` library allows you to serialize and deserialize Lua tables into JSON (JavaScript Object Notation(http://json.org)). You often use <nobr>JSON-formatted</nobr> data in conjunction with web APIs, for example requests sent using the facebook library.",
  type = "lib"
 },
 lfs = {
  childs = {},
  description = "__LuaFileSystem__ provides functionality to perform various <nobr>file-related</nobr> tasks, including but not limited to:",
  type = "lib"
 },
 licensing = {
  childs = {
   init = {
    args = "( providerName )",
    description = "Initializes the licensing service library by specifying the name of the licensing provider. Currently, only `\"google\"` is supported.",
    returns = "(Boolean)",
    type = "function"
   },
   verify = {
    args = "( listener )",
    description = "Starts the licensing verification process.",
    returns = "()",
    type = "function"
   }
  },
  description = "The Corona licensing library lets you check to see if the app was bought from a store. Currently, only Google Play(http://developer.android.com/google/play/licensing/setting-up.html) is supported.",
  type = "lib"
 },
 math = {
  childs = {
   abs = {
    args = "( x )",
    description = "Returns the absolute value (magnitude) of `x`.",
    returns = "(Number)",
    type = "function"
   },
   acos = {
    args = "( x )",
    description = "Returns the arc cosine of `x` in radians. The result will be in the range `0` to `pi`. If the parameter `x` is outside the range `-1..1`, the result will be `NaN`.",
    returns = "(Number)",
    type = "function"
   },
   asin = {
    args = "( x )",
    description = "Returns the arc sine of a `x` (in radians). The result will be in the range `-pi/2` to `pi/2`. If the parameter `x` is outside the range `-1..1`, the result will be `NaN`.",
    returns = "(Number)",
    type = "function"
   },
   atan = {
    args = "( x )",
    description = "Returns the arc tangent of a `x` in radians. The result will be in the range `-pi/2` to `pi/2`.",
    returns = "(Number)",
    type = "function"
   },
   atan2 = {
    args = "( y, x )",
    description = "Returns the arc tangent of `y/x` (in radians), but uses the signs of both parameters to find the quadrant of the result. It also correctly handles the case of `x` being `0`.",
    returns = "(Number)",
    type = "function"
   },
   ceil = {
    args = "( x )",
    description = "Returns the integer larger than or equal to `x`.",
    returns = "(Number)",
    type = "function"
   },
   cos = {
    args = "( x )",
    description = "Returns the cosine of `x` (the angle in radians). The result is a number in the range `-1, 1`.",
    returns = "(Number)",
    type = "function"
   },
   cosh = {
    args = "( x )",
    description = "Returns the hyperbolic cosine of `x`.",
    returns = "(Number)",
    type = "function"
   },
   deg = {
    args = "( x )",
    description = "Convert a value from radians to degrees.",
    returns = "(Number)",
    type = "function"
   },
   exp = {
    args = "( x )",
    description = "Returns the value `e^x^`.",
    returns = "(Number)",
    type = "function"
   },
   floor = {
    args = "( x )",
    description = "Returns the integer smaller than or equal to `x`.",
    returns = "(Number)",
    type = "function"
   },
   fmod = {
    args = "( x, y )",
    description = "Returns the remainder of the division of `x` by `y` that rounds the quotient towards zero.",
    returns = "(Number)",
    type = "function"
   },
   frexp = {
    args = "( x )",
    description = "Splits `x` into a normalized fraction and an exponent.",
    returns = "(Number)",
    type = "function"
   },
   huge = {
    description = "A value larger than or equal to any other numerical value.",
    type = "value"
   },
   inf = {
    description = "Returns a value larger than or equal to any other numerical value.",
    type = "value"
   },
   ldexp = {
    args = "( m, e )",
    description = "Returns `m*2^e^` where `m` containts the significant binary digits and `e` is the exponent.",
    returns = "(Number)",
    type = "function"
   },
   log = {
    args = "( x )",
    description = "Returns the natural logarithm of `x`.",
    returns = "(Number)",
    type = "function"
   },
   log10 = {
    args = "( x )",
    description = "Returns the base-10 logarithm of `x`.",
    returns = "(Number)",
    type = "function"
   },
   max = {
    args = "( x, ... )",
    description = "Returns the maximum value among its arguments.",
    returns = "(Number)",
    type = "function"
   },
   min = {
    args = "( x, ... )",
    description = "Return the minimum value among its arguments.",
    returns = "(Number)",
    type = "function"
   },
   modf = {
    args = "( x )",
    description = "Returns two numbers, the integral part of `x` and the fractional part of `x`.",
    returns = "(Numbers)",
    type = "function"
   },
   pi = {
    description = "Returns the constant _pi_.",
    type = "value"
   },
   pow = {
    args = "( x, y )",
    description = "Returns `x^y^`. (You can also use the expression `x^y` to compute this value.)",
    returns = "(Number)",
    type = "function"
   },
   rad = {
    args = "( x )",
    description = "Converts to radians an angle given in degrees.",
    returns = "(Number)",
    type = "function"
   },
   random = {
    args = "()",
    description = "Returns a pseudo-random number from a sequence with uniform distribution.",
    returns = "(Number)",
    type = "function"
   },
   randomseed = {
    args = "( seed )",
    description = "Sets the \"seed\" for the pseudo-random number generator. The same seed will produce the same sequence of random numbers each time and can be useful for testing but it is generally more desirable to have a different sequence for each run by using something like the current time as the seed (see below).",
    returns = "()",
    type = "function"
   },
   round = {
    args = "( x )",
    description = "Rounds number to the nearest integer following the same rules as the JavaScript version, i.e. if the fractional portion of number is `0.5` or greater, the argument is rounded to the next higher integer. If the fractional portion of number is less than `0.5`, the argument is rounded to the next lower integer.",
    returns = "(Number)",
    type = "function"
   },
   sin = {
    args = "( x )",
    description = "Returns the sine of `x` (the angle in radians). The result is a number in the range `-1, 1`.",
    returns = "(Number)",
    type = "function"
   },
   sinh = {
    args = "( x )",
    description = "Returns the hyperbolic sine of `x`.",
    returns = "(Number)",
    type = "function"
   },
   sqrt = {
    args = "( x )",
    description = "Returns the square root of `x` (equivalent to the expression `x^0.5`).",
    returns = "(Number)",
    type = "function"
   },
   tan = {
    args = "( x )",
    description = "Returns the tangent of `x` (the angle in radians).",
    returns = "(Number)",
    type = "function"
   },
   tanh = {
    args = "( x )",
    description = "Returns the hyperbolic tangent of `x`.",
    returns = "(Number)",
    type = "function"
   }
  },
  description = "This set of math function uses the standard Lua 5.1 math library (which calls the standard C library).",
  type = "lib"
 },
 media = {
  childs = {
   RemoteSource = {
    description = "Used to indicate that a file path is to be interpreted as a URL to a remote server. See native.newVideo() for a usage example.",
    type = "value"
   },
   capturePhoto = {
    args = "( { listener [, destination] } )",
    description = "Opens a platform-specific interface to the device's camera. This function is asynchronous, meaning that it returns immediately so the calling code will continue to execute until the end of its scope; after that, the application will be suspended until the session is complete. By default, the image object is added to the top of the current stage, although there is an option to save the image to a directory instead.",
    returns = "()",
    type = "function"
   },
   captureVideo = {
    args = "( { listener [, preferredQuality] [, preferredMaxDuration] } )",
    description = "Opens a platform-specific interface to the device's camera. This function is asynchronous, meaning that it returns immediately so the calling code will continue to execute until the end of its scope; after that, the application will be suspended until the session is complete. A listener is required to handle the captured object (URL) which can be used in native.newVideo().",
    returns = "()",
    type = "function"
   },
   getSoundVolume = {
    args = "()",
    description = "Returns the current volume setting for playback of extended sounds (media.playSound()).",
    returns = "(Number)",
    type = "function"
   },
   hasSource = {
    args = "( mediaSource )",
    description = "Determines if the given media source, such as a camera or photo library, is available on the platform. This function should be called before calling media.selectPhoto() to determine if that media service is available. Returns `true` if the media source is available on the platform/device. Returns `false` if not.",
    returns = "(Boolean)",
    type = "function"
   },
   newEventSound = {
    args = "( audioFileName [, baseDir] )",
    description = "Loads the event sound <nobr>(1-3 seconds)</nobr> from a sound file and returns an event sound ID that can be passed to media.playEventSound().",
    returns = "(Userdata)",
    type = "function"
   },
   newRecording = {
    args = "( [path] )",
    description = "Create an object for audio recording (Recording).",
    returns = "(Recording)",
    type = "function",
    valuetype = "_Recording"
   },
   pauseSound = {
    args = "()",
    description = "Pauses playback of the extended sound currently opened by the previous call to media.playSound() to resume playback of a paused sound.",
    returns = "()",
    type = "function"
   },
   playEventSound = {
    args = "( sound [, baseDir] [, completionListener] )",
    description = "Because of various limitations outlined below, it is recommended that the audio library be used for playing back audio.",
    returns = "()",
    type = "function"
   },
   playSound = {
    args = "( soundfile [, baseDir] [, onComplete] )",
    description = "Because of various limitations outlined below, it is recommended that the audio library be used for playing back audio.",
    returns = "()",
    type = "function"
   },
   playVideo = {
    args = "( path [, baseSource ], showControls, listener )",
    description = "Plays the video at the specified path (both local and remote) in a device-specific popup media player.",
    returns = "()",
    type = "function"
   },
   save = {
    args = "( filename [, baseDir] )",
    description = "Adds specified file to photo library.",
    returns = "()",
    type = "function"
   },
   selectPhoto = {
    args = "( { listener [, mediaSource] [, destination] [, origin] [, permittedArrowDirections] } )",
    description = "Opens a platform-specific interface to the device's photo library. This function is asynchronous, meaning that it returns immediately so the calling code will continue to execute until the end of its scope; after that, the application will be suspended until the session is complete. By default, the image object is added to the top of the current stage, although there is an option to save the image to a directory instead.",
    returns = "()",
    type = "function"
   },
   selectVideo = {
    args = "( { listener [, mediaSource] [, origin] [, permittedArrowDirections] } )",
    description = "Opens a platform-specific interface to the device's photo library. This function is asynchronous, meaning that it returns immediately so the calling code will continue to execute until the end of its scope; after that, the application will be suspended until the session is complete.",
    returns = "()",
    type = "function"
   },
   setSoundVolume = {
    args = "( volume )",
    description = "Adjusts the playback volume of an extended sound (media.playSound()). This setting can be adjusted at any time before or during the extended sound playback.",
    returns = "()",
    type = "function"
   },
   show = {
    args = "()",
    description = "This function is deprecated. Use media.selectPhoto() instead.",
    returns = "()",
    type = "function"
   },
   stopSound = {
    args = "()",
    description = "Stops playback of the extended sound currently opened by the previous call to media.playSound().",
    returns = "()",
    type = "function"
   }
  },
  description = "The media library provides access to the multimedia features of the device, including audio, video, camera, and the photo library.",
  type = "lib"
 },
 native = {
  childs = {
   canShowPopup = {
    args = "( name )",
    description = "Returns whether or not the popup type can be shown. This usually defines whether the popup will actually be displayed. However, in the case of `\"appStore\"`, a result of `true` does not guarantee that the popup will be displayed because, in those cases, the particular popup will depend on additional parameters.",
    returns = "(Boolean)",
    type = "function"
   },
   cancelAlert = {
    args = "( alert )",
    description = "Dismisses an alert box programmatically. For example, you may wish to have a popup alert that automatically disappears after ten seconds even if the user doesn't click it. In that case, you could call this function at the end of a timer.",
    returns = "()",
    type = "function"
   },
   cancelWebPopup = {
    args = "()",
    description = "Dismisses the currently displaying web popup. This function takes no arguments because only one web popup can be shown at one time (not to be confused with native web views which can have multiple instances shown at once).",
    returns = "()",
    type = "function"
   },
   getFontNames = {
    args = "()",
    description = "Returns an array of the available native fonts.",
    returns = "(Array)",
    type = "function"
   },
   getProperty = {
    args = "( key )",
    description = "Gets the value of a platform-specific property.",
    returns = "()",
    type = "function"
   },
   getSync = {
    args = "( filename, params )",
    description = "Gets the iCloud automatic backup settings for files in the `system.DocumentsDirectory` and `system.ApplicationSupportDirectory` on macOS and iOS systems.",
    returns = "(Boolean)",
    type = "function"
   },
   newFont = {
    args = "( name [, size] )",
    description = "Creates a font object that you can use to specify fonts in native text fields, native text boxes, and text display objects.",
    returns = "(Userdata)",
    type = "function"
   },
   newMapView = {
    args = "( x, y, width, height )",
    description = "Renders a map view within the specified boundaries and returns a display object wrapper. On the map view, touch events are available when you add a mapLocation event listener.",
    returns = "(Map)",
    type = "function",
    valuetype = "_Map"
   },
   newTextBox = {
    args = "( x, y, width, height )",
    description = "Creates a scrollable, __multi-line__ TextBox.",
    returns = "(TextBox)",
    type = "function",
    valuetype = "_TextBox"
   },
   newTextField = {
    args = "( x, y, width, height )",
    description = "Creates a __single-line__ TextField.",
    returns = "(TextField)",
    type = "function",
    valuetype = "_TextField"
   },
   newVideo = {
    args = "( x, y, width, height )",
    description = "Returns a video object that can be moved and rotated. This API supports local videos (in one of the system directories) or from a remote location (streaming).",
    returns = "(Video)",
    type = "function",
    valuetype = "_Video"
   },
   newWebView = {
    args = "( x, y, width, height )",
    description = "Loads a web page in a web view container. This native web view can be moved via `x`/`y` properties in the same manner as other display objects. On iOS, you can also rotate it via the `rotation` property.",
    returns = "(WebView)",
    type = "function",
    valuetype = "_WebView"
   },
   requestExit = {
    args = "()",
    description = "On Android, this closes the application window gracefully without terminating the process. On macOS and Windows, this closes the application.",
    returns = "()",
    type = "function"
   },
   setActivityIndicator = {
    args = "( state )",
    description = "Displays or hides a platform-specific activity indicator. Touch events are ignored while the indicator is shown.",
    returns = "()",
    type = "function"
   },
   setKeyboardFocus = {
    args = "( textField )",
    description = "Sets keyboard focus on a native.newTextField() to the native object's listener function.",
    returns = "()",
    type = "function"
   },
   setProperty = {
    args = "( key, value )",
    description = "Sets a platform-specific property.",
    returns = "()",
    type = "function"
   },
   setSync = {
    args = "( filename, params )",
    description = "Sets the iCloud automatic backup flag for files in the `system.DocumentsDirectory` and `system.ApplicationSupportDirectory` on macOS and iOS systems.",
    returns = "(Boolean)",
    type = "function"
   },
   showAlert = {
    args = "( title, message [, buttonLabels [, listener] ] )",
    description = "Displays a popup alert box with one or more buttons, using a native alert control. Program activity, including animation, will continue in the background, but all other user interactivity will be blocked until the user selects a button or cancels the dialog.",
    returns = "(Object)",
    type = "function"
   },
   showPopup = {
    args = "( name )",
    description = "Displays the operating system's default popup window for a specified service. Displaying this popup suspends the app on both iOS and Android.",
    returns = "(Boolean)",
    type = "function"
   },
   showWebPopup = {
    args = "( [x, y, width, height,] url, [options] )",
    description = "Creates a web popup that loads a local or remote web page.",
    returns = "()",
    type = "function"
   },
   systemFont = {
    description = "The native system font used by default for display.newText().",
    type = "value"
   },
   systemFontBold = {
    description = "The native \"bold\" system font used by default for display.newText().",
    type = "value"
   }
  },
  description = "The native library wraps platform-specific UI elements like input fields, maps, web views, and more. These are controlled/rendered by the OS, not the Corona engine.",
  type = "lib"
 },
 network = {
  childs = {
   canDetectNetworkStatusChanges = {
    description = "Returns `true` if network reachability events are supported on the current platform.",
    type = "value"
   },
   cancel = {
    args = "( requestId )",
    description = "Cancel an outstanding network request made with network.request().",
    returns = "()",
    type = "function"
   },
   download = {
    args = "( url, method, listener [, params], filename [, baseDirectory] )",
    description = "This API is a convenience method that is very similar to the asynchronous network.request(), putting the destination `filename` and `baseDirectory` parameters into `params.response` and specifying `\"download\"` progress notifications. ",
    returns = "(Userdata)",
    type = "function"
   },
   request = {
    args = "( url, method, listener [, params] )",
    description = "Makes an asynchronous HTTP or HTTPS request to a URL. This function returns a handle that can be passed to network.cancel() in order to cancel the request.",
    returns = "(Userdata)",
    type = "function"
   },
   setStatusListener = {
    args = "( hostURL, listener )",
    description = "Starts monitoring a host for its network reachability status. This API is designed around the idea that you are monitoring individual hosts and not the hardware directly. Potentially, some hosts might continue to be reachable while others are not due to internet conditions, firewall configurations, authentication rules, and whether you need full DNS routing.",
    returns = "()",
    type = "function"
   },
   upload = {
    args = "( url, method, listener [, params], filename [, baseDirectory] [, contentType] )",
    description = "This API is a convenience method that is very similar to the asynchronous network.request(), putting the source `filename` and `baseDirectory` parameters into a `params.body` table, adding `contentType` as a `params.headers` request header value, and specifying `\"upload\"` progress notifications. ",
    returns = "(Userdata)",
    type = "function"
   }
  },
  type = "lib"
 },
 os = {
  childs = {
   clock = {
    args = "()",
    description = "Returns an approximation of the amount in seconds of CPU time used by the program.",
    returns = "(Number)",
    type = "function"
   },
   date = {
    args = "( [format [, time] ] )",
    description = "Returns a string or a table containing date and time, formatted according to the given string format. When called without arguments, `os.date()` returns a reasonable date and time representation that depends on the host system and on the current locale (that is, `os.date()` is equivalent to `os.date(\"%c\")`).",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   difftime = {
    args = "( newerTime, olderTime )",
    description = "Returns the number of seconds from time `t1` to time `t2`. In POSIX, Windows, and some other systems, this value is exactly <nobr>`t2 - t1`</nobr>.",
    returns = "(Number)",
    type = "function"
   },
   execute = {
    args = "( cmd )",
    description = "Passes a string to the operating system for execution and returns a system-dependent status code. This function is equivalent to the C function `system()`. ",
    returns = "(Number)",
    type = "function"
   },
   exit = {
    args = "( [ exit ] )",
    description = "Calls the C function `exit()`, with an optional code, to terminate the host program. The default value for code is the success code.",
    returns = "()",
    type = "function"
   },
   remove = {
    args = "( file )",
    description = "Deletes a file or directory and returns two possible values:",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   rename = {
    args = "( oldname, newname )",
    description = "Renames a file or directory and returns two possible values:",
    returns = "(Boolean)",
    type = "function"
   },
   time = {
    args = "( table )",
    description = "Returns the current time in seconds from 1970 when called without arguments, or a time representing the date and time specified by the given table.",
    returns = "(Number)",
    type = "function"
   }
  },
  description = "This standard Lua library provides functions for dealing with system time and date and other OS-related functions.",
  type = "lib"
 },
 package = {
  childs = {
   loaded = {
    description = "A table used by `require()` to control which modules are already loaded. When you require a module `modname` and `package.loadedmodname` is not `false`, `require()` simply returns the value stored there.",
    type = "value"
   },
   loaders = {
    description = "A table used by `require()` to control how to load modules.",
    type = "value"
   },
   require = {
    args = "( \"moduleName\" )",
    description = "Loads the given module. ",
    returns = "(Library)",
    type = "function"
   }
  },
  description = "Corona supports Lua's module functionality for creating and loading external libraries. You can create your own libraries and call them from your application.",
  type = "lib"
 },
 physics = {
  childs = {
   addBody = {
    args = "( object, [bodyType,] [params] )",
    description = "This function turns almost any Corona display object into a simulated physical object with specific physical properties. It accepts the display object, an optional body type (string), and an optional `params` table containing <nobr>key-value</nobr> pairs that specify the properties for the physics body. Within this `params` table, the following apply:",
    returns = "(Boolean)",
    type = "function"
   },
   engineVersion = {
    description = "A string value representing the Box2D and LiquidFun engine version.",
    type = "value"
   },
   fromMKS = {
    args = "( unitName, value )",
    description = "Convenience function for converting from MKS units to Corona units.",
    returns = "(Number)",
    type = "function"
   },
   getAverageCollisionPositions = {
    args = "()",
    description = "It's common for Box2D to report multiple contact points during a single iteration of a simulation. This function is use to determine if averaging of all the contact points is enabled.",
    returns = "(Boolean)",
    type = "function"
   },
   getDebugErrorsEnabled = {
    args = "()",
    description = "This function is used to determine if extra physics errors are allowed to be caught by Box2D.",
    returns = "(Boolean)",
    type = "function"
   },
   getGravity = {
    args = "()",
    description = "Returns the __x__ and __y__ components of the global gravity vector, in units of m/s2. This takes advantage of the fact that Lua functions can return multiple values, which in this case are:",
    returns = "(Numbers)",
    type = "function"
   },
   getMKS = {
    args = "( key )",
    description = "Get the MKS value of the physics simulation for specific keys.",
    returns = "(Number)",
    type = "function"
   },
   getReportCollisionsInContentCoordinates = {
    args = "()",
    description = "This function is used to determine if the content origin is the collision point in the collision physics events.",
    returns = "(Boolean)",
    type = "function"
   },
   getTimeScale = {
    args = "()",
    description = "Returns the physics time flow multiplier set via physics.setTimeScale().",
    returns = "(Number)",
    type = "function"
   },
   newJoint = {
    args = "( jointType, ... )",
    description = "Joints can be used to assemble complex physical objects from multiple rigid bodies. For example, joints can be used to join the limbs of a ragdoll figure, attach the wheels of a vehicle to its body, create a moving elevator platform, and more.",
    returns = "(Joint)",
    type = "function",
    valuetype = "_Joint"
   },
   newParticleSystem = {
    args = "( params )",
    description = "A ParticleSystem(http://google.github.io/liquidfun/) framework.",
    returns = "(ParticleSystem)",
    type = "function",
    valuetype = "_ParticleSystem"
   },
   pause = {
    args = "()",
    description = "Pause the physics engine.",
    returns = "()",
    type = "function"
   },
   queryRegion = {
    args = "( upperLeftX, upperLeftY, lowerRightX, lowerRightY )",
    description = "This function is used to find the objects that intersect with an axis-aligned (non-rotated) box. This box is defined by an <nobr>upper-left</nobr> coordinate and a <nobr>lower-right</nobr> coordinate.",
    returns = "()",
    type = "function"
   },
   rayCast = {
    args = "( fromX, fromY, toX, toY, behavior )",
    description = "This function is used to find the objects that collide with a line, and the collision points along that line.",
    returns = "()",
    type = "function"
   },
   reflectRay = {
    args = "( fromX, fromY, hit )",
    description = "This function is used to reflect a vector as returned by physics.rayCast().",
    returns = "()",
    type = "function"
   },
   removeBody = {
    args = "( object )",
    description = "Removes a physics body from a display object without destroying the entire object. This removes the body created with physics.addBody().",
    returns = "(Boolean)",
    type = "function"
   },
   setAverageCollisionPositions = {
    args = "( enabled )",
    description = "Because it's common for Box2D to report multiple collision points during a single frame iteration, this setting is useful to report those points as one averaged point. This function applies to all the contact points in the collision physics events.",
    returns = "()",
    type = "function"
   },
   setContinuous = {
    args = "( enabled )",
    description = "By default, Box2D performs continuous collision detection, which prevents objects from \"tunneling.\" If it were turned off, an object that moves quickly enough could potentially pass through a thin wall.",
    returns = "()",
    type = "function"
   },
   setDebugErrorsEnabled = {
    args = "( enabled )",
    description = "This function allows extra physics errors to be caught by Box2D.",
    returns = "()",
    type = "function"
   },
   setDrawMode = {
    args = "( mode )",
    description = "Sets one of three possible \"rendering modes\" for the physics engine. While this feature will run on devices, it's most useful in the Corona Simulator when testing/debugging physics behavior.",
    returns = "()",
    type = "function"
   },
   setGravity = {
    args = "( gx, gy )",
    description = "Sets the __x__ and __y__ components of the global gravity vector in units of m/s2. The default is `( 0, 9.8 )` to simulate standard Earth gravity, pointing downward on the __y__ axis.",
    returns = "()",
    type = "function"
   },
   setMKS = {
    args = "( key, value )",
    description = "Sets the MKS (meters, kilograms, and seconds) value of the physics simulation for specific keys. This is strictly for advanced purposes ??? the average developer and project will not require this function.",
    returns = "()",
    type = "function"
   },
   setPositionIterations = {
    args = "( value )",
    description = "Sets the accuracy of the engine's position calculations. The default value is `3`.",
    returns = "()",
    type = "function"
   },
   setReportCollisionsInContentCoordinates = {
    args = "( enabled )",
    description = "This function sets the content origin as the collision point in the collision physics events.",
    returns = "()",
    type = "function"
   },
   setScale = {
    args = "( value )",
    description = "Sets the internal pixels-per-meter ratio that is used in converting between on-screen Corona coordinates and simulated physics coordinates. This should be done only once, before any physical objects are instantiated.",
    returns = "()",
    type = "function"
   },
   setTimeScale = {
    args = "( scale )",
    description = "Adjusts the physics simulation time flow with a multiplier.",
    returns = "()",
    type = "function"
   },
   setTimeStep = {
    args = "( dt )",
    description = "Specifies either a frame-based (approximated) physics simulation or a <nobr>time-based</nobr> simulation.",
    returns = "()",
    type = "function"
   },
   setVelocityIterations = {
    args = "( value )",
    description = "Sets the accuracy of the engine's velocity calculations. The default value is `8`.",
    returns = "()",
    type = "function"
   },
   start = {
    args = "()",
    description = "This function start the physics simulation and should be called before any other physics functions.",
    returns = "()",
    type = "function"
   },
   stop = {
    args = "()",
    description = "Stops the physics engine. This function will return `false` and display a warning message if the API cannot be processed.",
    returns = "(Boolean)",
    type = "function"
   },
   toMKS = {
    args = "( unitName, value )",
    description = "Convenience function for converting from Corona units to MKS units.",
    returns = "(Number)",
    type = "function"
   }
  },
  description = "Physics are commonly used for apps that involve a simulation of objects that move, collide, and interact under various physical forces like gravity. Corona makes it very easy to add physics to your apps, even if you've never worked with a physics engine before. While the underlying engine is built around the popular Box2D, we've taken a different design approach which eliminates a lot of the coding that is traditionally required.",
  type = "lib"
 },
 productList = {
  childs = {
   invalidProducts = {
    description = "An array (table) of strings representing a list of unavailable/invalid product identifiers.",
    type = "value"
   },
   name = {
    description = "The string `\"productList\"`.",
    type = "value"
   },
   products = {
    description = "Lua array (table) where each element is itself a Lua table containing the following product information:",
    type = "value"
   }
  },
  description = "The following event properties are passed to the listener function specified in store.loadProducts().",
  type = "lib"
 },
 socket = {
  childs = {},
  description = "The `socket` library provides low-level access to the network stack on the host device. It is __not__ intended for general networking usage (the network APIs will generally continue to work because it is directly supported by device vendors.",
  type = "lib"
 },
 sqlite3 = {
  childs = {},
  description = "Corona includes support for SQLite databases on all platforms. The documentation for LuaSQLite can be viewed here(http://luasqlite.luaforge.net/lsqlite3.html).",
  type = "lib"
 },
 store = {
  childs = {
   canLoadProducts = {
    description = "This property will be `true` if the store supports the loading of products. Following confirmation, the store.loadProducts() function can be used to retrieve information about items available for sale.",
    type = "value"
   },
   canMakePurchases = {
    description = "On iOS, there is a setting which disables <nobr>in-app</nobr> purchasing, commonly used to prevent children from purchasing items without permission. Use this property to check in advance if purchasing is allowed and notify the user if purchasing is forbidden.",
    type = "value"
   },
   finishTransaction = {
    args = "( transaction )",
    description = "Notifies the Apple Store that a transaction is complete.",
    returns = "()",
    type = "function"
   },
   init = {
    args = "( listener )",
    description = "This call is required and must be executed before making other Apple IAP calls. This prepares the Apple IAP library and, upon successful initialization, sets store.isActive to `true`.",
    returns = "()",
    type = "function"
   },
   isActive = {
    description = "Boolean. This will be `true` upon successful initialization, `false` otherwise.",
    type = "value"
   },
   loadProducts = {
    args = "( productIdentifiers, productListener )",
    description = "Initiates a request to retrieve item data, dispatching a productList event to the listener defined as `productListener`.",
    returns = "()",
    type = "function"
   },
   purchase = {
    args = "( productIdentifiers )",
    description = "Initiates a purchase transaction on provided product(s) by sending out a purchase request to the store, then dispatches a storeTransaction.",
    returns = "()",
    type = "function"
   },
   restore = {
    args = "()",
    description = "Users who wipe the information on a device or buy a new device may wish to restore previously purchased items. This function initiates the process of retrieving all valid purchases.",
    returns = "()",
    type = "function"
   },
   target = {
    description = "Indicates the store that the app was built for. It will yield one of the following strings:",
    type = "value"
   }
  },
  description = "This library allows you to support <nobr>in-app</nobr> purchasing on iOS, including <nobr>in-game</nobr> currency, upgrades, and more.",
  type = "lib"
 },
 storeTransaction = {
  childs = {
   name = {
    description = "The string `\"storeTransaction\"`.",
    type = "value"
   },
   transaction = {
    description = "This table contains the following <nobr>read-only</nobr> properties pertaining to the transaction:",
    type = "value"
   }
  },
  description = "The following event properties are passed to the listener function specified in store.init().",
  type = "lib"
 },
 string = {
  childs = {
   byte = {
    args = "( s [, i [, j]] )",
    description = "Returns the internal numerical codes of the characters in a string.",
    returns = "(Number)",
    type = "function"
   },
   char = {
    args = "( [arg1 [, ...] )",
    description = "Returns a string in which each character has the internal numerical code equal to its corresponding argument.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   ends = {
    args = "( s, suffix )",
    description = "Returns a boolean `true` or `false` depending on whether a given string ends with the specified `suffix` characters.",
    returns = "(Boolean)",
    type = "function"
   },
   find = {
    args = "( s, pattern [, init [, plain]] )",
    description = "Looks for the first match of a pattern in a string. If found, it returns the indices where the occurrence starts and ends; otherwise, returns `nil`.",
    returns = "(Numbers)",
    type = "function"
   },
   format = {
    args = "( formatstring [, ...] )",
    description = "Returns a formatted string following the description given in its arguments.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   gmatch = {
    args = "( s, pattern )",
    description = "Returns a pattern-finding iterator. See Lua String Manipulation for more information.",
    returns = "()",
    type = "function"
   },
   gsub = {
    args = "( s, pattern, repl [, n] )",
    description = "Replaces all occurrences of a pattern in a string. Returns the string after the substitutions have been performed, along with the number of substitutions made <nobr>(`0` if none)</nobr>.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   len = {
    args = "( s )",
    description = "Returns the length of a string (amount of characters).",
    returns = "(Number)",
    type = "function"
   },
   lower = {
    args = "( s )",
    description = "Change uppercase characters in a string to lowercase.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   match = {
    args = "( s, pattern [, init] )",
    description = "Extract substrings by matching a pattern in a string. If a match is found, returns the captures from the pattern; otherwise returns `nil`. If pattern specifies no captures, then the whole match is returned.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   rep = {
    args = "( s, n )",
    description = "Replicates a string by returning a string that is the concatenation of `n` copies of a specified String.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   reverse = {
    args = "( s )",
    description = "Reverses a string.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   starts = {
    args = "( s, prefix )",
    description = "Returns a boolean `true` or `false` depending on whether a given string starts with the specified `prefix` characters.",
    returns = "(Boolean)",
    type = "function"
   },
   sub = {
    args = "( s, i [, j] )",
    description = "Returns a substring (a specified portion of an existing string).",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   upper = {
    args = "( s )",
    description = "Change lowercase characters in a string to uppercase. All other characters are left unchanged.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   }
  },
  description = "This library provides generic functions for string manipulation, such as finding and extracting substrings, and pattern matching. This uses the standard Lua 5.1 library.",
  type = "lib"
 },
 system = {
  childs = {
   ApplicationSupportDirectory = {
    description = "Used with system.pathForFile() to create a path for storing and retrieving files that need to persist between application sessions but are not visible to users. For example, this might be used for storing downloadable levels for a game.",
    type = "value"
   },
   CachesDirectory = {
    description = "Used with system.pathForFile() to create a path for storing and retrieving files that are available across application launches. This is ideal for saving state information. ",
    type = "value"
   },
   DocumentsDirectory = {
    description = "Used with system.pathForFile() to create a path for storing and retrieving files that need to persist between application sessions. The path is `/Documents`.",
    type = "value"
   },
   ResourceDirectory = {
    description = "Used with system.pathForFile() to create a path for retrieving files where all the application assets exist, for example image and sound files. This is often called the \"app bundle.\"",
    type = "value"
   },
   TemporaryDirectory = {
    description = "Used with system.pathForFile() to create a path for storing and retrieving files that only need to persist while the application is running. The path is `/tmp`.",
    type = "value"
   },
   activate = {
    args = "( feature )",
    description = "Activates a system-level feature such as `\"multitouch\"`. Similarly, you can use system.deactivate() to disable a feature.",
    returns = "()",
    type = "function"
   },
   canOpenURL = {
    args = "( url )",
    description = "Returns a boolean.",
    returns = "(Boolean)",
    type = "function"
   },
   cancelNotification = {
    args = "()",
    description = "Local/push notifications have been moved to the Notifications plugin. Please see the documentation for usage details.",
    returns = "()",
    type = "function"
   },
   deactivate = {
    args = "( feature )",
    description = "Deactivates a system-level feature such as `\"multitouch\"`.",
    returns = "()",
    type = "function"
   },
   deletePreferences = {
    args = "( category, preferenceNames )",
    description = "Deletes preferences from storage.",
    returns = "(Boolean)",
    type = "function"
   },
   getIdleTimer = {
    args = "()",
    description = "Returns whether the application idle timer is enabled. ",
    returns = "(Boolean)",
    type = "function"
   },
   getInfo = {
    args = "( property )",
    description = "Returns information about the system on which the application is running.",
    returns = "()",
    type = "function"
   },
   getInputDevices = {
    args = "()",
    description = "Returns an array of InputDevice devices that are currently connected to the system, such as a touchscreen, keyboard, mouse, joystick, etc. Alternatively returns an empty array if no input devices are found. Note that input devices which were once connected but were later disconnected will not be returned by this function.",
    returns = "(InputDevice)",
    type = "function",
    valuetype = "_InputDevice"
   },
   getPreference = {
    args = "( category, name [, type] )",
    description = "Returns the requested preference's value.",
    returns = "()",
    type = "function"
   },
   getTimer = {
    args = "()",
    description = "Returns time in milliseconds since application launch. The fractional part of the returned value may return microseconds if the hardware supports it.",
    returns = "(Number)",
    type = "function"
   },
   hasEventSource = {
    args = "( eventName )",
    description = "Returns a boolean `true` or `false` depending on whether the system delivers events corresponding to `eventName`.",
    returns = "(Boolean)",
    type = "function"
   },
   newEventDispatcher = {
    args = "()",
    description = "Returns a new EventDispatcher function. ",
    returns = "(EventDispatcher)",
    type = "function",
    valuetype = "_EventDispatcher"
   },
   openURL = {
    args = "( url )",
    description = "Open a web page in the browser, create an email, or dial a phone number. Executing this function will close the app and switch to the <nobr>built-in</nobr> browser, email, or phone app.",
    returns = "(Boolean)",
    type = "function"
   },
   orientation = {
    description = "String value identifying the orientation of the device:",
    type = "value"
   },
   pathForFile = {
    args = "( filename [, baseDirectory] )",
    description = "Generates an absolute path using system-defined directories as the base. An additional optional parameter, `baseDirectory`, specifies which base directory is used to construct the full path, with its default value being `system.ResourceDirectory`.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   scheduleNotification = {
    args = "()",
    description = "Local/push notifications have been moved to the Notifications plugin. Please see the documentation for usage details.",
    returns = "(Userdata)",
    type = "function"
   },
   setAccelerometerInterval = {
    args = "( frequency )",
    description = "Sets the frequency of accelerometer events.",
    returns = "()",
    type = "function"
   },
   setGyroscopeInterval = {
    args = "( frequency )",
    description = "Sets the frequency of gyroscope events in Hz. Gyroscope events are a significant drain on battery, so only increase the frequency when you need faster responses. ",
    returns = "()",
    type = "function"
   },
   setIdleTimer = {
    args = "( enabled )",
    description = "Controls whether the idle timer is enabled. If set to `true`, the timer will be active (default) or inactive if `false`. When active, the idle timer dims the screen and eventually puts the device to sleep when no user activity occurs.",
    returns = "()",
    type = "function"
   },
   setLocationAccuracy = {
    args = "( distance )",
    description = "Sets the desired accuracy of location (GPS) events to distance in meters. Note: the actual accuracy depends on the capabilities of the device and/or platform. On the iPhone, accuracy is limited to discrete distances: < 10, 10, 100, 1000, and 3000 meters. Higher accuracy (smaller distances) requires more battery life, so use larger distance to preserve battery life.",
    returns = "()",
    type = "function"
   },
   setLocationThreshold = {
    args = "( distance )",
    description = "Sets how much distance in meters must be travelled until the next location (GPS) event is sent. Because location events involve hardware that can drain the battery, using larger threshold distances preserve battery life.",
    returns = "()",
    type = "function"
   },
   setPreferences = {
    args = "( category, preferences )",
    description = "Writes a table of preference values to storage. If any of the given preferences do not exist in storage, they will be inserted. If any of the given preferences already exist in storage, they will be overwritten.",
    returns = "(Boolean)",
    type = "function"
   },
   setTapDelay = {
    args = "( delayTime )",
    description = "The delay time between when a tap is detected and when the tap event is delivered. By default, this time is 0.",
    returns = "()",
    type = "function"
   },
   vibrate = {
    args = "()",
    description = "Vibrates the phone. In the Corona Simulator, this will sound a system beep (macOS only).",
    returns = "()",
    type = "function"
   }
  },
  description = "Collectively, the system functions listed here return system information and control system functions ??? get device information, enable multitouch, control the idle time, accelerometer, GPS, etc.",
  type = "lib"
 },
 table = {
  childs = {
   concat = {
    args = "( t )",
    description = "Concatenate the elements of a table together to form a string. Each element must be able to be coerced into a string. A separator can be specified which is placed between concatenated elements.",
    returns = "(String)",
    type = "function",
    valuetype = "string"
   },
   copy = {
    args = "( t )",
    description = "Returns a shallow copy of array, i.e. the portion of the array (table) with integer keys. A variable number of additional arrays can be passed in as optional arguments. If an array has a hole (a `nil` entry), copying in a given source array stops at the last consecutive item prior to the hole.",
    returns = "(Array)",
    type = "function"
   },
   indexOf = {
    args = "( t, element )",
    description = "Returns the integer index of an element within an array, or `nil` if the element is not in the array. The search goes through the length of the array as determined by `#t` whose value is undefined if there are holes.",
    returns = "(Number)",
    type = "function"
   },
   insert = {
    args = "( t, value )",
    description = "Inserts a given value into a table. If a position is given, inserts the value before the element currently at that position.",
    returns = "()",
    type = "function"
   },
   maxn = {
    args = "( t )",
    description = "Returns the largest positive numerical index of the given table, or zero if the table has no positive numerical indices. To do its job, this function does a linear traversal of the whole table.",
    returns = "(Number)",
    type = "function"
   },
   remove = {
    args = "( t )",
    description = "Removes from table the element at position `pos`, shifting down other elements to close the space, if necessary. Returns the value of the removed element. ",
    returns = "()",
    type = "function"
   },
   sort = {
    args = "( array [, compare] )",
    description = "Sorts table elements in a given order, in-place, from `table1` to `tablen`, where `n` is the length of the table. It receives the table (array) to be sorted plus an optional `compare` function. This `compare` function receives two arguments and must return `true` if the first argument should come first in the sorted array. If `compare` is __not__ given, then the standard Lua operator `<` is used instead.",
    returns = "()",
    type = "function"
   }
  },
  description = "Tables in Lua implement associative arrays. That is, they can be indexed not just with numbers, but also with strings or any other value of the language, except nil.",
  type = "lib"
 },
 timer = {
  childs = {
   allowIterationsWithinFrame = {
    description = "Changes the behavior of the timer object to provide events as soon as possible instead of waiting until the next frame to execute. The default behavior is `false` meaning that timers fire on frame intervals. Setting this to `true` will enable the events to arrive as soon as possible.",
    type = "value"
   },
   cancel = {
    args = "( timerID )",
    description = "Cancels a timer operation initiated with timer.performWithDelay().",
    returns = "()",
    type = "function"
   },
   pause = {
    args = "( timerId )",
    description = "Pauses a timer started with timer.performWithDelay() that represents the amount of time remaining, in milliseconds.",
    returns = "(Number)",
    type = "function"
   },
   performWithDelay = {
    args = "( delay, listener [, iterations] )",
    description = "Calls a specified function after a delay. This function returns a table to cancel the invocation of the specified listener.",
    returns = "(Table)",
    type = "function"
   },
   resume = {
    args = "( timerId )",
    description = "Resumes a timer that was paused with timer.pause() that represents the amount of time remaining in the timer.",
    returns = "(Number)",
    type = "function"
   }
  },
  description = "Timer functions allow calling a function some time in the future rather than immediately. The timer library provides the basic functions to let set up the timer and manage the event listener.",
  type = "lib"
 },
 transition = {
  childs = {
   blink = {
    args = "( target, params )",
    description = "Blinks (glows) an object in and out over a specified time, repeating indefinitely.",
    returns = "(Object)",
    type = "function"
   },
   cancel = {
    args = "()",
    description = "The `transition.cancel()` function will cancel one of the following, depending on the passed parameter:",
    returns = "()",
    type = "function"
   },
   dissolve = {
    args = "( object1, object2, time, delay )",
    description = "Performs a dissolve transition between two display objects.",
    returns = "(Object)",
    type = "function"
   },
   fadeIn = {
    args = "( target, params )",
    description = "Fades an object to alpha of `1.0` over the specified time.",
    returns = "(Object)",
    type = "function"
   },
   fadeOut = {
    args = "( target, params )",
    description = "Fades an object to alpha of `0.0` over the specified time.",
    returns = "(Object)",
    type = "function"
   },
   from = {
    args = "( target, params )",
    description = "Similar to transition.to() except that the starting property values are specified in the parameters table and the final values are the corresponding property values of the object prior to the call.",
    returns = "(Table)",
    type = "function"
   },
   moveBy = {
    args = "( target, params )",
    description = "Moves an object by the specified `x` and `y` coordinate amount over a specified time.",
    returns = "(Object)",
    type = "function"
   },
   moveTo = {
    args = "( target, params )",
    description = "Moves an object to the specified `x` and `y` coordinates over a specified time.",
    returns = "(Object)",
    type = "function"
   },
   pause = {
    args = "()",
    description = "The `transition.pause()` function pauses one of the following, depending on the passed parameter:",
    returns = "()",
    type = "function"
   },
   resume = {
    args = "()",
    description = "The `transition.resume()` function resumes one of the following, depending on the passed parameter:",
    returns = "()",
    type = "function"
   },
   scaleBy = {
    args = "( target, params )",
    description = "Scales an object by the specified `xScale` and `yScale` amounts over a specified time.",
    returns = "(Object)",
    type = "function"
   },
   scaleTo = {
    args = "( target, params )",
    description = "Scales an object to the specified `xScale` and `yScale` amounts over a specified time.",
    returns = "(Table)",
    type = "function"
   },
   to = {
    args = "( target, params )",
    description = "Animates (transitions) a display object algorithm. Use this to move, rotate, fade, or scale an object over a specific period of time.",
    returns = "(Table)",
    type = "function"
   }
  },
  description = "The transition library provides functions and methods to transition/tween display objects or display groups over a specific period of time. Library features include:",
  type = "lib"
 },
 widget = {
  childs = {
   newButton = {
    args = "( options )",
    description = "Creates a ButtonWidget object.",
    returns = "(ButtonWidget)",
    type = "function",
    valuetype = "_ButtonWidget"
   },
   newPickerWheel = {
    args = "( options )",
    description = "Creates a PickerWheelWidget object.",
    returns = "(PickerWheelWidget)",
    type = "function",
    valuetype = "_PickerWheelWidget"
   },
   newProgressView = {
    args = "( options )",
    description = "Creates a ProgressViewWidget object.",
    returns = "(ProgressViewWidget)",
    type = "function",
    valuetype = "_ProgressViewWidget"
   },
   newScrollView = {
    args = "( options )",
    description = "Creates a ScrollViewWidget object.",
    returns = "(ScrollViewWidget)",
    type = "function",
    valuetype = "_ScrollViewWidget"
   },
   newSegmentedControl = {
    args = "( options )",
    description = "Creates a SegmentedControlWidget object.",
    returns = "(SegmentedControlWidget)",
    type = "function",
    valuetype = "_SegmentedControlWidget"
   },
   newSlider = {
    args = "( options )",
    description = "Creates a SliderWidget object.",
    returns = "(SliderWidget)",
    type = "function",
    valuetype = "_SliderWidget"
   },
   newSpinner = {
    args = "( options )",
    description = "Creates a SpinnerWidget object.",
    returns = "(SpinnerWidget)",
    type = "function",
    valuetype = "_SpinnerWidget"
   },
   newStepper = {
    args = "( options )",
    description = "Creates a StepperWidget object. The stepper consists of a minus and plus button which can be tapped or held down to decrement/increment a value, for example, the music or sound volume setting in a game.",
    returns = "(StepperWidget)",
    type = "function",
    valuetype = "_StepperWidget"
   },
   newSwitch = {
    args = "( options )",
    description = "Creates a SwitchWidget object.",
    returns = "(SwitchWidget)",
    type = "function",
    valuetype = "_SwitchWidget"
   },
   newTabBar = {
    args = "( options )",
    description = "Creates a TabBarWidget object.",
    returns = "(TabBarWidget)",
    type = "function",
    valuetype = "_TabBarWidget"
   },
   newTableView = {
    args = "( options )",
    description = "Creates a TableViewWidget object.",
    returns = "(TableViewWidget)",
    type = "function",
    valuetype = "_TableViewWidget"
   },
   setTheme = {
    args = "( theme )",
    description = "Use this function to set the overall theme of the widget library. This should be called immediately after you `require()` the widget library.",
    returns = "()",
    type = "function"
   }
  },
  type = "lib"
 }
}

-- when loaded as a package, return the package; otherwise continue with the script
if pcall(debug.getlocal, 4, 1) then return api end

-- The followng script is used to convert Corona API descriptions to api/lua/corona.lua.

package.path = package.path .. ';../../lualibs/?/?.lua;../../lualibs/?.lua'
package.cpath = package.cpath .. ';../../bin/clibs/?.dll'

local mobdebug = require('mobdebug')
local lfs = require('lfs')
local DIR_SEP = '/'
local API = {}

local function startswith(s, piece)
  return string.sub(s, 1, string.len(piece)) == piece
end

local function beforeComma(line)
  for i = 1, line:len() do
    if line:sub(i, i) == '.' then
      return line:sub(1, i - 1)
    end
  end
  return line
end

local function stripTags(line)
  return line:gsub('%[api.+]', '')
  :gsub('(%[.-%])%[.-%]','%1')
  :gsub('[%]%[]', '')
  :gsub('&nbsp;', ' ')
  :gsub('&mdash;', '-')
  :gsub('&minus;', '-')
  :gsub('&[lr]dquo;', '"')
  :gsub('&sup(%d);', '%1')
  :gsub('???', "'") -- replace unicode quote
end

local function extractOverview(filename)
  local overviewInd, parent, overview
  local i = 0
  local f = io.open(filename, 'r')
  for l in f:lines() do
    i = i + 1
    if l == '## Overview' or l:sub(1, 1) == '<' then
      overviewInd = i + 2
    elseif l:find("^> __Parent__") then
      parent = l:match("%[(.-)%]")
    elseif i == overviewInd then
      if l:sub(1, 1) == '!' then
        overviewInd = i + 2
      else
        overview = stripTags(l)
        break
      end
    end
  end
  f:close()
  return overview, parent
end

local function isSpecialType(r)
  return (r
    and r ~= 'String'
    and r ~= 'Boolean'
    and r ~= 'Number'
    and r ~= 'Numbers'
    and r ~= 'Constant'
    and r ~= 'Function'
    and r ~= 'Array'
    and r ~= 'Table'
    and r ~= 'TYPE'
    and r ~= 'Object'
    and r ~= 'Library'
    and r ~= 'Module'
    and r ~= 'CoronaClass'
    and r ~= 'Event'
    and r ~= 'Listener'
    and r ~= 'Userdata')
end

local function extractTypeArgsReturns(filename)
  local t, a, r
  local syntaxInd
  local i = 0
  local f = io.open(filename, 'r')
  for l in f:lines() do
    i = i + 1
    if startswith(l, '> __Type__') then
      t = l:match('%[api%.type%.(%a*)%]', 1):lower()
    elseif startswith(l, '> __Return value__') then
      r = l:match('%[(%a*)%]', 1)
    elseif l == '## Syntax' then
      syntaxInd = i + 2
    elseif i == syntaxInd then
      a = l:match('%(.+%)', 1)
    end
    if t and a and r then
      return t, a, r
    end
  end
  f:close()
  return t or '', a or '', r or ''
end

-- generate special valuetype names to avoid conflicts
-- with variable names that may have different capitalization
local function specialType(r) return '_'..r end

local function processApiDir(root, kind, item)
  for entity in lfs.dir(root) do
    if entity:sub(1, 1) ~= '.' then
      local fullPath = root .. DIR_SEP .. entity
      local mode = lfs.attributes(fullPath, 'mode')
      if mode == 'file' and item then
        if entity == 'index.markdown' then
          local description, parent = extractOverview(fullPath)
          API[item].description = description
          if parent and isSpecialType(parent) then API[item].inherits = specialType(parent) end
        else
          local t, a, r = extractTypeArgsReturns(fullPath)
          if t ~= 'function' then
            t, a, r = 'value', nil, nil
          elseif kind == 'class' then
            t = 'method'
          end
          local child = {type = t, description = extractOverview(fullPath), args = a, returns = r}
          if r and r ~= '' and isSpecialType(r) then
            child.valuetype = specialType(r)
          elseif r == 'String' then
            child.valuetype = 'string'
          end
          if t == 'function' or t == 'method' then
            child.args = a and #a > 1 and a or '()'
            child.returns = '('..(r or '')..')'
          end
          API[item].childs[beforeComma(entity)] = child
        end
      elseif mode == 'directory' and entity ~= 'global' then
        if kind == 'class' then entity = specialType(entity) end
        API[entity] = {type = kind, description = '', childs = {}}
        processApiDir(fullPath, kind, entity)
      end
    end
  end
end
processApiDir('library', 'lib')
processApiDir('type', 'class')

print(mobdebug.line(API, {indent = ' ', comment = false}))
