# bare-image-resample

Image resampling support for Bare.

```
npm i bare-image-resample
```

## Usage

```js
const jpeg = require('bare-jpeg')
const { resize } = require('bare-image-resample')

const image = require('./my-image.jpg', { with: { type: 'binary' } })

const decoded = jpeg.decode(image)
// {
//   width: 200,
//   height: 400,
//   data: <Buffer>
// }

const resized = resize(decoded, 100, 200)
// {
//   width: 100,
//   height: 200,
//   data: <Buffer>
// }
```

## License

Apache-2.0
