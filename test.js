const test = require('brittle')
const jpeg = require('bare-jpeg')
const { resize } = require('.')

test('resize .jpg', (t) => {
  const image = require('./test/fixtures/grapefruit.jpg', {
    with: { type: 'binary' }
  })

  const resized = resize(jpeg.decode(image), 100)

  t.comment(resized)
})
