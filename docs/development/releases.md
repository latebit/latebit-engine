Releases
---

Releases are handled manually by creating a tag in Github.

## Creating a release

1. Go [here](https://github.com/shikaan/latebit/releases)
2. Click "Create a new release"
3. Input `va.b.c-d.e` as release title
4. Whenever appropriate, use the `pre-release` box

## Versioning

This project follows [semantic versioning](http://semver.org/). A complete
version string will look something like `v1.2.3-xxx.4` with the following
meaning

- first digit (in the example, `1`) is incremented in presence of breaking changes;
- second digit (in the example, `2`) is incremented when adding a new feature;
- third digit (in the example, `2`) is incremented a fix is introduced;
- the `-xxx.4` part is optional and reserved to pre-releases
