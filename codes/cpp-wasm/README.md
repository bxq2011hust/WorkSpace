
```bash
emcc capi_js.cc --js-library pkg.js -o capi_js.js
emcc closure.cc --js-library closure_pkg.js -o closure.js
```

```bash
python -m SimpleHTTPServer 8080
```

```bash
http://127.0.0.1:8080/capi_js.html
http://127.0.0.1:8080/closure.html
```

