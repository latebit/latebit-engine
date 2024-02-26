Logging Guidelines
---

### Example

```cpp
Log.debug("Class::method(): What happened. Further information")
```

### Message Structure

1. The prefix should always be `Class::method():`
2. The first chunk is capitalised
3. The second chunk is optional
4. There is no `.` at the end
5. Messages should be short (max 1024 chars) and searcheable