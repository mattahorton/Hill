Hill
====

A musical and visual accompaniment to spoken word poetry


###Instructions
Place 'poem.json' and audio file in 'Data' directory.

Press 'space' to start poem.

###Poem Schema

```javascript
{
  "track": "<path>/<to>/<wavfile.wav>",
  "lines": [
    {
      "text": "1st line",
      "time": 3, // time in seconds. Can use decimals.
      "cam" : { // Provide camera position for this line
        "r" : 700, // Distance away from the center of the scene
        "angle" : 0.674740942 // Angle around the x-z circle
      }
    },
    {
      "text":"Line 2",
      "time":5,
      "cam" : {
        "r": 900,
        "angle" : 0.357889
      },
      "offset" : { // Provide offsets for text movement
        // All text is originally 500 units close to
        // the center of the scene, and 100 units
        // "up" (positive y axis).
        // Location is measured from the bottom
        // right of the text object.
        "z" : -400, // Can provide x, y, and z movements
        "x" : -1000 // Each offset axis is optional
      }
    },
    {
      // If no "cam" object is provided
      // the previous camera location will
      // be used. "cam" objects must contain
      // an "r" and "angle" element.
      "text":"Melisma",
      "time":2
      "offset" : {
        "y" : -100,
        "z" : 200
      }
    }
  ]
}
```
